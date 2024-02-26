#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <cassert>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <stdlib.h>
#include <string>

#include "toojpeg/toojpeg.h"

#include "doctest/doctest.h"

#include "miniz/miniz.h"

#include "sha256/SHA256.h"
#include "sha256/sha256_util.h"

#include "sd19/bits2bytes.h"
#include "sd19/getnset.h"
#include "sd19/readmis.h"
#include "sd19/readrast.h"
#include "sd19/syserr.h"

#include "sd19/fatalerr.h"
#include "sd19db/dbmanager.h"

#include "toojpeg/toojpeg_helper.h"

TEST_CASE("toojpeg create file")
{
    const auto width = 800;
    const auto height = 600;
    // RGB: one byte each for red, green, blue
    const auto bytesPerPixel = 3;
    // allocate memory
    auto image = new unsigned char[width * height * bytesPerPixel];
    // create a nice color transition (replace with your code)
    for (auto y = 0; y < height; y++) {
        for (auto x = 0; x < width; x++) {
            // memory location of current pixel
            auto offset = (y * width + x) * bytesPerPixel;
            // red and green fade from 0 to 255, blue is always 127
            image[offset] = 255 * x / width;
            image[offset + 1] = 255 * y / height;
            image[offset + 2] = 127;
        }
    }
    const bool isRGB = true;        // true = RGB image, else false = grayscale
    const auto quality = 90;        // compression quality: 0 = worst, 100 = best, 80 to 90 are most often used
    const bool downsample = false;  // false = save as YCbCr444 JPEG (better quality), true = YCbCr420 (smaller file)
    const char* comment = "TooJpeg example image"; // arbitrary JPEG comment

    bool ret = TooJpeg::save_jpeg("temp.jpg", image, width, height, bytesPerPixel, isRGB, quality, downsample, "TooJpeg example image");
    CHECK(ret == true);
    std::remove("temp.jpg");

    delete[] image;    
}

TEST_CASE("ihead and hsfpage and mis - can insert 100 rows")
{
    return;

    using namespace sdb19db;

    srand(time(NULL));

    std::remove("db.db3");

    DbManager dbm("db.db3");

    for (int i = 0; i < 100; i++)
    {
        tables::ihead ihead_row;
        ihead_row.created    = "hello";
        ihead_row.par_x      = 2;
        ihead_row.parent     = "asdfadsf";
        
        const int ihead_id = dbm.Insert(ihead_row);        
        CHECK((i+1) == ihead_id);
        
        tables::hsfpage hsfpage_row;
        hsfpage_row.hsf_num             = rand() % 9;
        hsfpage_row.ihead_id            = ihead_id;
        hsfpage_row.writer_num          = rand() % 4170;
        hsfpage_row.template_num        = rand() % 100;        
        hsfpage_row.image_len_bytes     = (i + 1);        
        hsfpage_row.image               = new char[hsfpage_row.image_len_bytes];
        std::fill(hsfpage_row.image, hsfpage_row.image + hsfpage_row.image_len_bytes, (i+1));

        const int hsfpage_id = dbm.Insert(hsfpage_row);
        CHECK((i + 1) == hsfpage_id);

        delete[] hsfpage_row.image;

        tables::mis mis_row;
        mis_row.hsf_num         = rand() % 9;
        mis_row.ihead_id        = ihead_id;
        mis_row.writer_num      = rand() % 4170;
        mis_row.template_num    = rand() % 100;
        mis_row.character       = char(rand() % 26 + 65);
        mis_row.image_len_bytes = (i + 1);
        mis_row.image           = new char[mis_row.image_len_bytes];
        std::fill(mis_row.image, mis_row.image + mis_row.image_len_bytes, (i + 1));

        const int mis_id = dbm.Insert(mis_row);
        CHECK((i + 1) == mis_id);

        delete[] mis_row.image;
    }
}

TEST_CASE("ihead and hsfpage - insert and read")
{
    return;
    using namespace sdb19db;

    DbManager dbm("db.db3");

    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

    for (const auto& dirEntry : recursive_directory_iterator("hsf_page"))
    {
        if (!dirEntry.is_directory()
            && (dirEntry.path().string().find("template") == std::string::npos)
            && (dirEntry.path().string().find("truerefs") == std::string::npos)
        )
        {
            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

            //f0000_14.pct
            std::string hsfdirname(dirEntry.path().parent_path().filename().string());
            hsfdirname = std::regex_replace(hsfdirname, std::regex("hsf_page"), "");
            hsfdirname = std::regex_replace(hsfdirname, std::regex("hsf_"), "");

            const std::string filepath(dirEntry.path().string());
            const std::string filename(dirEntry.path().filename().string());
            const std::string writer(filename.substr(1, 4));
            const std::string templ(filename.substr(6, 2));

            //------------------------------------------------------------------------
            //TODO: reading the hsf page file twice
            //      1) get_file_sha256_checksum
            //      2) ReadBinaryRaster
            //------------------------------------------------------------------------
            const std::string hsfpage_sha256(get_file_sha256_checksum(filepath));

            if (!dbm.HsfPageProcessed(hsfpage_sha256)) 
            {
                std::cout
                    << " processing: "  << filepath
                    << " writer: "      << writer
                    << " template: "    << templ
                    << " dirname: "     << hsfdirname
                    << std::endl;

                IHEAD* head;
                unsigned char* buf;
                int width, height, bpi;
                char* data8;
                const std::string tempImageFileName("temp.pct.png");

                ReadBinaryRaster((char*)filepath.c_str(), &head, &buf, &bpi, &width, &height);

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

                const int ihead_id = dbm.Insert(ihead_row);            

                tables::hsfpage hsfpage_row;
                hsfpage_row.hsf_page_sha256     = hsfpage_sha256;
                hsfpage_row.hsf_num             = std::stoi(hsfdirname);
                hsfpage_row.ihead_id            = ihead_id;
                hsfpage_row.writer_num          = std::stoi(writer);
                hsfpage_row.template_num        = std::stoi(templ);
                hsfpage_row.image_len_bytes     = png_data_size;
                hsfpage_row.image               = (char*)pPNG_data;
                hsfpage_row.image_sha256        = SHA256::toString(digest);

                const int hsfpage_id = dbm.Insert(hsfpage_row);

                delete[] image;
                mz_free(pPNG_data);
                free(data8);
                free(head);
            }
            else
            {
                std::cout
                    << " skipping: "    << filepath
                    << " writer: "      << writer
                    << " template:"     << templ
                    << " dirname: "     << hsfdirname
                    << std::endl;
            }
        }
    }
}

TEST_CASE("ihead and mis - insert and read")
{    
    using namespace sdb19db;

    //std::remove("db.db3");
    DbManager dbm("db.db3");

    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

    for (const auto& dirEntry : recursive_directory_iterator("by_write"))
    {
        std::string hsf_num(dirEntry.path().parent_path().parent_path().string());
        hsf_num = std::regex_replace(hsf_num, std::regex("by_write"), "");
        hsf_num = std::regex_replace(hsf_num, std::regex("hsf_"), "");
        hsf_num = std::regex_replace(hsf_num, std::regex("\\\\"), "");
        const std::string fileext (dirEntry.path().extension().string());

        // by_write\hsf_0\f0039_14\l0039_14.mis
        if (!dirEntry.is_directory() && 
            (0 == fileext.compare(".mis")))
        {
            const std::string filepath(dirEntry.path().string());
            const std::string filename(dirEntry.path().filename().string());

            std::string clsfilename(filename);
            clsfilename = std::regex_replace(clsfilename, std::regex(".mis"), ".cls");
            std::string clsfilepath(filepath);
            clsfilepath = std::regex_replace(clsfilepath, std::regex(filename), clsfilename);
            
            const std::string writer(filename.substr(1, 4));
            const std::string templ(filename.substr(6, 2));

            MIS* mis;
            char* data8, * dptr;
            int misentry = 0;

            mis = readmisfile((char*)filepath.c_str());
            //------------------------------------------------------------------------
            //TODO: reading the MIS file twice.
            //      1) readmisfile
            //      2) get_file_sha256_checksum
            //------------------------------------------------------------------------
            const std::string mis_sha256(get_file_sha256_checksum(filepath));

            if (!dbm.MisProcessed(mis_sha256))
            {
                //read in the cls file
                std::ifstream clsfile(clsfilepath);

                std::vector<std::string> mischars = {};
                if (clsfile.is_open()) {
                    std::string line;
                    std::getline(clsfile, line);
                    int lineint = std::stoi(line);
                    if (mis->ent_num != lineint) {
                        std::cerr << ".mis and .cls counts do not match: " << filepath << " numcls: " << lineint << " nummis: " << mis->ent_num;
                        exit(1);
                    }
                    //each line is a hex value represiting the ascii char
                    while (std::getline(clsfile, line)) {
                        unsigned int i;                    
                        std::istringstream iss(line);
                        iss >> std::hex >> i;
                        //null terminate the array or it will get garbage
                        char s[] = { i, 0x00 };
                        mischars.push_back(std::string(s));                    
                    }
                    clsfile.close();                
                }

                if (mis->misd != 1) {
                    fatalerr("show_mis", "", "incorrect entry size or depth");
                }
                if ((data8 = (char*)malloc(mis->misw * mis->mish * sizeof(char))) == NULL) {
                    syserr("show_mis", "malloc", "unable to allocate 8 bit space");
                }

                std::cout 
                    << " processing: "  << filepath 
                    << " writer: "      << writer
                    << " template: "    << templ
                    << " hsf_num: "     << hsf_num
                    << " clsfilepath: " << clsfilepath
                    << " characters: "  << mischars.size()
                    << std::endl;

                // converts ALL of the mis data to bytes in one go
                bits2bytes(mis->data, (u_char*)data8, mis->misw * mis->mish);

                for (dptr = data8, misentry = 0; misentry < mis->ent_num; misentry++)
                {
                    //std::cout << misentry << " character: " << mischars.at(misentry) << std::endl;
                
                    const auto width = mis->entw;
                    const auto height = mis->enth;                
                    const auto bytesPerPixel = 1;
                    auto image = new unsigned char[width * height * bytesPerPixel];

                    //TooJpeg::misdata_to_bwimage(dptr, image, mis->entw, mis->enth, 1);

                    for (int k = 0; k < mis->enth; k++)
                    {
                        for (int l = 0; l < mis->entw; l++)
                        {
                            auto offset = (k * width + l) * bytesPerPixel;
                            // red and green fade from 0 to 255, blue is always 127
                            auto red = 255 * l / width;
                            auto green = 255 * k / height;
                            //image[offset] = (red + green) / 2;;
                            image[offset] = (*dptr++) ? 255 : 0;
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

                    const int ihead_id      = dbm.Insert(ihead_row);            

                    tables::mis mis_row;
                    mis_row.mis_sha256          = mis_sha256;
                    mis_row.hsf_num             = std::stoi(hsf_num);
                    mis_row.ihead_id            = ihead_id;
                    mis_row.writer_num          = std::stoi(writer);
                    mis_row.template_num        = std::stoi(templ);
                    mis_row.character           = mischars.at(misentry);
                    mis_row.image_len_bytes     = png_data_size;
                    mis_row.image               = (char*)pPNG_data;
                    mis_row.image_sha256        = SHA256::toString(digest);
                    
                    const int mis_id            = dbm.Insert(mis_row);
                    mz_free(pPNG_data);
                }
                //outer loop
                //data8 contains ALL the mis files
                free(data8);
                free(mis);
            }
            else
            {
                std::cout
                    << " skipping: "    << filepath 
                    << " writer: "      << writer
                    << " template: "    << templ
                    << " hsf_num: "     << hsf_num
                    << " clsfilepath: " << clsfilepath
                    << std::endl;
            }
        }
    }
}