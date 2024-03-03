#include <cassert>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <format>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <regex>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <thread>

#include "toojpeg/toojpeg.h"

#include "ulog/ulog.h"

#include "pugixml/pugixml.hpp"

#include "miniz/miniz.h"

#include "sha256/SHA256.h"
#include "sha256/sha256_util.h"

#include "sd19/bits2bytes.h"
#include "sd19/freemis.h"
#include "sd19/getnset.h"
#include "sd19/readmis.h"
#include "sd19/readrast.h"
#include "sd19/syserr.h"

#include "sd19/fatalerr.h"
#include "sd19db/dbmanager.h"

#include "sd19config.h"

#include "misprocessing.h"

#include "toojpeg/toojpeg_helper.h"
#include "sd19/normalize.h"


std::vector<MisInfo> GetMisFiles()
{
    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

    std::vector<MisInfo> entries;
    for (const auto& dirEntry : recursive_directory_iterator("by_write"))
    {
        std::string hsf_num(dirEntry.path().parent_path().parent_path().string());
        hsf_num = std::regex_replace(hsf_num, std::regex("by_write"), "");
        hsf_num = std::regex_replace(hsf_num, std::regex("hsf_"), "");
        hsf_num = std::regex_replace(hsf_num, std::regex("\\\\"), "");
        const std::string fileext (dirEntry.path().extension().string());

        // by_write\hsf_0\f0039_14\l0039_14.mis
        if (!dirEntry.is_directory()
            && (0 == fileext.compare(".mis")))
        {
		    const std::string filepath(dirEntry.path().string());
		    const std::string filename(dirEntry.path().filename().string());

		    std::string clsfilename(filename);
		    clsfilename = std::regex_replace(clsfilename, std::regex(".mis"), ".cls");
		    std::string clsfilepath(filepath);
		    clsfilepath = std::regex_replace(clsfilepath, std::regex(filename), clsfilename);
					
		    const std::string field_type(filename.substr(0, 1));
		    const std::string writer(filename.substr(1, 4));
		    const std::string templ(filename.substr(6, 2));    

            MisInfo mis;
            mis.filepath        = filepath;
            mis.filename        = filename;
            mis.hsf_num         = hsf_num;
            mis.clsfilepath     = clsfilepath;
            mis.clsfilename     = clsfilename;
            mis.writer          = writer;
            mis.templ           = templ;
            mis.field_type      = field_type;
            mis.mis_sha256      = get_file_sha256_checksum(mis.filepath);
            {
                std::lock_guard<std::mutex> guard(dbmutex);
                if (!dbm->MisProcessed(mis.mis_sha256)) {

                    //read in the cls file if the mis file has not been processed
                    std::ifstream clsfile(mis.clsfilepath);
                    if (clsfile.is_open()) {
                        std::string line;
                        std::getline(clsfile, line);
                        int lineint = std::stoi(line);

                        //each line is a hex value represiting the ascii char
                        while (std::getline(clsfile, line)) {
                            unsigned int i;
                            std::istringstream iss(line);
                            iss >> std::hex >> i;
                            //null terminate the array or it will get garbage
                            char s[] = { i, 0x00 };
                            mis.mischars.push_back(std::string(s));
                        }
                        clsfile.close();
                    }
                    entries.push_back(mis);
                }
            }
        }
    }
    return entries;
}

void process_mis_thread_callback(const MisInfo info)
{
    using namespace sdb19db;

    ulog(to_string(info));

    MIS* mis;
    char* data8, * dptr;
    int misentry = 0;
    {
        //-------------------------------------------------------------------------------------
        //TODO: something down in the decode code is not threadsafe
        //      saw registers and all sorts of bit math in code so not surprising
        //-------------------------------------------------------------------------------------
        {
            std::lock_guard<std::mutex> guard(readbinaryrastermutex);

            //--------------------------------------------------------------------
            //!BUG! readmisfile() - was clearning compression after it decompresses it
            //--------------------------------------------------------------------
            
            
            mis = readmisfile((char*)info.filepath.c_str());
            if (mis->misd != 1) {
                fatalerr("show_mis", "", "incorrect entry size or depth");
            }

            {
                MIS* mis2;
                float* scal_x;
                float* scal_y;
                norm_2nd_gen2(&mis2, &scal_x, &scal_y, mis);
                freemis(mis);
                mis = mis2;
            }
            if ((data8 = (char*)malloc(mis->misw * mis->mish * sizeof(char))) == NULL) {
                syserr("show_mis", "malloc", "unable to allocate 8 bit space");
            }
            bits2bytes(mis->data, (u_char*)data8, mis->misw * mis->mish);
            
            //mis = readmisfile((char*)info.filepath.c_str());
            //if (mis->misd != 1) {
            //    fatalerr("show_mis", "", "incorrect entry size or depth");
            //}
            //if ((data8 = (char*)malloc(mis->misw * mis->mish * sizeof(char))) == NULL) {
            //    syserr("show_mis", "malloc", "unable to allocate 8 bit space");
            //}
            //bits2bytes(mis->data, (u_char*)data8, mis->misw * mis->mish);
        }

        for (dptr = data8, misentry = 0; misentry < mis->ent_num; misentry++)
        {
            //std::cout << misentry << " character: " << mischars.at(misentry) << std::endl;            
                
            const auto width = mis->entw;
            const auto height = mis->enth;
            const auto bytesPerPixel = 1;
            auto image = new unsigned char[width * height * bytesPerPixel];

            for (int k = 0; k < mis->enth; k++)
            {
                for (int l = 0; l < mis->entw; l++)
                {
                    auto offset = (k * width + l) * bytesPerPixel;
                    // red and green fade from 0 to 255, blue is always 127
                    //auto red = 255 * l / width;
                    //auto green = 255 * k / height;
                    //image[offset] = (red + green) / 2;;
                    image[offset] = (*dptr++) ? 0 : 255;
                }
            }

            size_t png_data_size = 0;
            void* pPNG_data = tdefl_write_image_to_png_file_in_memory_ex(image, width, height, 1, &png_data_size, 6, MZ_FALSE);

            SHA256 sha;
            sha.update((uint8_t*)pPNG_data, png_data_size);
            std::array<uint8_t, 32> digest = sha.digest();

            tables::ihead ihead_row;
            ihead_row.created       = get_created(mis->head);
            ihead_row.width         = get_width(mis->head);
            ihead_row.height        = get_height(mis->head);
            ihead_row.depth         = get_depth(mis->head);
            ihead_row.density       = get_density(mis->head);
            ihead_row.compress      = get_compression(mis->head);
            ihead_row.complen       = get_complen(mis->head);
            ihead_row.align         = get_align(mis->head);
            ihead_row.unitsize      = get_unitsize(mis->head);
            ihead_row.sigbit        = get_sigbit(mis->head);
            ihead_row.byte_order    = get_byte_order(mis->head);
            ihead_row.pix_offset    = get_pix_offset(mis->head);
            ihead_row.whitepix      = get_whitepix(mis->head);
            ihead_row.issigned      = get_issigned(mis->head);
            ihead_row.rm_cm         = get_rm_cm(mis->head);
            ihead_row.tb_bt         = get_tb_bt(mis->head);
            ihead_row.lr_rl         = get_lr_rl(mis->head);
            ihead_row.parent        = get_parent(mis->head);
            ihead_row.par_x         = get_par_x(mis->head);
            ihead_row.par_y         = get_par_y(mis->head);

            tables::mis mis_row;
            if (0 == info.field_type.rfind("d", 0)) {
                mis_row.field_type = 0;
            }
            if (0 == info.field_type.rfind("u", 0)) {
                mis_row.field_type = 1;
            }
            if (0 == info.field_type.rfind("l", 0)) {
                mis_row.field_type = 2;
            }
            if (0 == info.field_type.rfind("c", 0)) {
                mis_row.field_type = 3;
            }
            mis_row.mis_sha256          = info.mis_sha256;
            mis_row.entry_num           = misentry;
            mis_row.hsf_num             = std::stoi(info.hsf_num);        
            mis_row.writer_num          = std::stoi(info.writer);
            mis_row.template_num        = std::stoi(info.templ);
            mis_row.character           = info.mischars.at(misentry);
            mis_row.image_len_bytes     = png_data_size;
            mis_row.image               = (char*)pPNG_data;
            mis_row.image_sha256        = SHA256::toString(digest);
            {
                std::lock_guard<std::mutex> guard(dbmutex);

                const int ihead_id      = dbm->Insert(ihead_row);            
                mis_row.ihead_id        = ihead_id;
                const int mis_id        = dbm->Insert(mis_row);
            }

            delete[] image;
            mz_free(pPNG_data);
        }   
        //outer loop
        //data8 contains ALL the mis files                
        free(data8);
        freemis(mis);
    }
}

std::ostream& operator<<(std::ostream& out, const MisInfo& mis_row)
{
	out << std::endl
		<< std::setw(20) << "filepath:"		<< std::setw(65) << mis_row.filepath		<< std::endl
		<< std::setw(20) << "filename:"		<< std::setw(65) << mis_row.filename		<< std::endl
		<< std::setw(20) << "clsfilename:"	<< std::setw(65) << mis_row.clsfilename		<< std::endl
		<< std::setw(20) << "clsfilepath:"	<< std::setw(65) << mis_row.clsfilepath     << std::endl
		<< std::setw(20) << "templ:"	    << std::setw(65) << mis_row.templ		    << std::endl
		<< std::setw(20) << "writer:"	    << std::setw(65) << mis_row.writer		    << std::endl
		<< std::setw(20) << "field_type:"	<< std::setw(65) << mis_row.field_type      << std::endl
		<< std::setw(20) << "hsf_num:"	    << std::setw(65) << mis_row.hsf_num         << std::endl
        << std::setw(20) << "mis_sha256:"	<< std::setw(65) << mis_row.mis_sha256		<< std::endl;        
	return out;
}

std::string to_string(const MisInfo& h) {
    std::ostringstream ss;
    ss << h;
    return ss.str();
}