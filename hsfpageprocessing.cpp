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

#include "hsfpageprocessing.h"

#include "toojpeg/toojpeg_helper.h"

std::vector<HsfPageInfo> GetHsfPages()
{
    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

    std::vector<HsfPageInfo> entries;
    for (const auto& dirEntry : recursive_directory_iterator("hsf_page"))
    {
        if (!dirEntry.is_directory()
            && (dirEntry.path().string().find("template") == std::string::npos)
            && (dirEntry.path().string().find("truerefs") == std::string::npos)
            )
        {
            //f0000_14.pct
            std::string hsfdirname(dirEntry.path().parent_path().filename().string());
            hsfdirname = std::regex_replace(hsfdirname, std::regex("hsf_page"), "");
            hsfdirname = std::regex_replace(hsfdirname, std::regex("hsf_"), "");

            HsfPageInfo hsf;
            hsf.filepath = std::string(dirEntry.path().string());
            hsf.hsfdirname = hsfdirname;
            hsf.filename = std::string(dirEntry.path().filename().string());
            hsf.writer = std::string(hsf.filename.substr(1, 4));
            hsf.templ = std::string(hsf.filename.substr(6, 2));
            hsf.hsfpage_sha256 = get_file_sha256_checksum(hsf.filepath);

            {
                std::lock_guard<std::mutex> guard(dbmutex);
                if (!dbm->HsfPageProcessed(hsf.hsfpage_sha256)) {
                    entries.push_back(hsf);
                }
            }
        }
    }
    return entries;
}

void process_hsf_page_thread_callback(const HsfPageInfo info)
{
    using namespace sdb19db;

    ulog(to_string(info));

    IHEAD* head;
    unsigned char* buf;
    int width, height, bpi;
    char* data8;
    
    {
        //-------------------------------------------------------------------------------------
        //TODO: something down in the decode code is not threadsafe
        //      saw registers and all sorts of bit math in code so not surprising
        //-------------------------------------------------------------------------------------
        std::lock_guard<std::mutex> guard(readbinaryrastermutex);
        ReadBinaryRaster((char*)info.filepath.c_str(), &head, &buf, &bpi, &width, &height);
    }

    if ((data8 = (char*)malloc(width * height * sizeof(char))) == NULL) {
        syserr("ReadBinaryRaster", "malloc", "unable to allocate 8 bit space");
    }
    bits2bytes(buf, (u_char*)data8, width * height);

    auto image = new unsigned char[width * height * 1];
    TooJpeg::misdata_to_bwimage(data8, image, width, height, 1);

    size_t png_data_size = 0;
    void* pPNG_data = tdefl_write_image_to_png_file_in_memory_ex(image, width, height, 1, &png_data_size, 6, MZ_FALSE);
    SHA256 sha;
    sha.update((uint8_t*) pPNG_data, png_data_size);
    std::array<uint8_t, 32> digest = sha.digest();
    
    tables::ihead ihead_row;
    ihead_row.created       = get_created(head);
    ihead_row.width         = get_width(head);
    ihead_row.height        = get_height(head);
    ihead_row.depth         = get_depth(head);
    ihead_row.density       = get_density(head);
    ihead_row.compress      = get_compression(head);
    ihead_row.complen       = get_complen(head);
    ihead_row.align         = get_align(head);
    ihead_row.unitsize      = get_unitsize(head);
    ihead_row.sigbit        = get_sigbit(head);
    ihead_row.byte_order    = get_byte_order(head);
    ihead_row.pix_offset    = get_pix_offset(head);
    ihead_row.whitepix      = get_whitepix(head);
    ihead_row.issigned      = get_issigned(head);
    ihead_row.rm_cm         = get_rm_cm(head);
    ihead_row.tb_bt         = get_tb_bt(head);
    ihead_row.lr_rl         = get_lr_rl(head);
    ihead_row.parent        = get_parent(head);
    ihead_row.par_x         = get_par_x(head);
    ihead_row.par_y         = get_par_y(head);
    {
        std::lock_guard<std::mutex> guard(dbmutex);
        const int ihead_id = dbm->Insert(ihead_row);            

        tables::hsfpage hsfpage_row;
        hsfpage_row.hsf_page_sha256     = info.hsfpage_sha256;
        hsfpage_row.hsf_num             = std::stoi(info.hsfdirname);
        hsfpage_row.ihead_id            = ihead_id;
        hsfpage_row.writer_num          = std::stoi(info.writer);
        hsfpage_row.template_num        = std::stoi(info.templ);
        hsfpage_row.image_len_bytes     = png_data_size;
        hsfpage_row.image               = (char*)pPNG_data;
        hsfpage_row.image_sha256        = SHA256::toString(digest);

        const int hsfpage_id = dbm->Insert(hsfpage_row);
    }

    delete[] image;
    mz_free(pPNG_data);
    free(data8);
    free(head);
}

std::ostream& operator<<(std::ostream& out, const HsfPageInfo& hsr_row)
{
	out << std::endl
		<< std::setw(20) << "filepath:"		    << std::setw(65) << hsr_row.filepath		    << std::endl
		<< std::setw(20) << "filename:"		    << std::setw(65) << hsr_row.filename		    << std::endl
		<< std::setw(20) << "templ:"	        << std::setw(65) << hsr_row.templ		        << std::endl
		<< std::setw(20) << "writer:"	        << std::setw(65) << hsr_row.writer		        << std::endl
		<< std::setw(20) << "hsfdirname:"	    << std::setw(65) << hsr_row.hsfdirname          << std::endl
		<< std::setw(20) << "hsfpage_sha256:"   << std::setw(65) << hsr_row.hsfpage_sha256    << std::endl;        
	return out;
}

std::string to_string(const HsfPageInfo& h)
{
    std::ostringstream ss;
    ss << h;
    return ss.str();
}