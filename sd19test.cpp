#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <cassert>
#include <filesystem>
#include <iostream>
#include <memory>
#include <regex>
#include <stdlib.h>
#include <string>

#include "toojpeg/toojpeg.h"

#include "doctest/doctest.h"

#include "sd19/bits2bytes.h"
#include "sd19/getnset.h"
#include "sd19/readmis.h"
#include "sd19/readrast.h"
#include "sd19/syserr.h"

#include "sd19db/dbmanager.h"

// output file
std::ofstream myFile("example.jpg", std::ios_base::out | std::ios_base::binary);
// write a single byte compressed by tooJpeg
void myOutput(unsigned char byte)
{
    myFile << byte;
}

TEST_CASE("toojpeg create file")
{
    // 800x600 image
    const auto width = 800;
    const auto height = 600;
    // RGB: one byte each for red, green, blue
    const auto bytesPerPixel = 3;
    // allocate memory
    auto image = new unsigned char[width * height * bytesPerPixel];
    // create a nice color transition (replace with your code)
    for (auto y = 0; y < height; y++)
        for (auto x = 0; x < width; x++)
        {
            // memory location of current pixel
            auto offset = (y * width + x) * bytesPerPixel;
            // red and green fade from 0 to 255, blue is always 127
            image[offset] = 255 * x / width;
            image[offset + 1] = 255 * y / height;
            image[offset + 2] = 127;
        }
    // start JPEG compression
    // note: myOutput is the function defined in line 18, it saves the output in example.jpg
    // optional parameters:
    const bool isRGB = true;  // true = RGB image, else false = grayscale
    const auto quality = 90;    // compression quality: 0 = worst, 100 = best, 80 to 90 are most often used
    const bool downsample = false; // false = save as YCbCr444 JPEG (better quality), true = YCbCr420 (smaller file)
    const char* comment = "TooJpeg example image"; // arbitrary JPEG comment
    auto ok = TooJpeg::writeJpeg(myOutput, image, width, height, isRGB, quality, downsample, comment);
    CHECK(ok == true);
    delete[] image;
}

TEST_CASE("ihead and hsfpage - can insert 100 rows")
{
    srand(time(NULL));

    using namespace sdb19db;

    std::remove("db.db3");

    DbManager dbm("db.db3");
    dbm.Setup();

    for (int i = 0; i < 100; i++)
    {
        tables::ihead ihead_row;
        ihead_row.created    = "hello";
        ihead_row.par_x      = 2;
        ihead_row.parent     = "asdfadsf";
        
        const int ihead_id = dbm.Insert(ihead_row);        
        CHECK((i+1) == ihead_id);
        
        tables::hsfpage hsfpage_row;
        hsfpage_row.hsf_num      = rand() % 9;
        hsfpage_row.ihead_id     = ihead_id;
        hsfpage_row.writer_num   = rand() % 4170;
        hsfpage_row.template_num = rand() % 100;
        hsfpage_row.buffer_len_bytes = (i+1);
        //hsfpage_row.buffer           = (char*)malloc(hsfpage_row.buffer_len_bytes);
        hsfpage_row.buffer           = new char[hsfpage_row.buffer_len_bytes];
        std::fill(hsfpage_row.buffer, hsfpage_row.buffer + hsfpage_row.buffer_len_bytes, (i+1));

        const int hsfpage_id = dbm.Insert(hsfpage_row);

        delete[] hsfpage_row.buffer;

        CHECK((i + 1) == hsfpage_id);
    }
}

TEST_CASE("ihead and hsfpage - insert and read")
{
    using namespace sdb19db;

    std::remove("db.db3");
    DbManager dbm("db.db3");

    dbm.Setup();

    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

    for (const auto& dirEntry : recursive_directory_iterator("hsf_page"))
    {
        if (!dirEntry.is_directory())
        {
            //f0000_14.pct
            std::string hsfdirname(dirEntry.path().parent_path().filename().string());
            hsfdirname = std::regex_replace(hsfdirname, std::regex("hsf_page"), "");
            hsfdirname = std::regex_replace(hsfdirname, std::regex("hsf_"), "");

            const std::string filepath(dirEntry.path().string());
            const std::string filename(dirEntry.path().filename().string());
            const std::string writer(filename.substr(1, 4));
            const std::string templ(filename.substr(6, 2));

            std::cout 
                << " processing: "  << filepath 
                << " writer: "      << writer
                << " template: "    << templ
                << " dirname: "     << hsfdirname
                << std::endl;

            IHEAD* head;
            unsigned char* buf;
            int misw, mish, bpi, entw, enth, ent_num;
            char* data8, * dptr;
            int j = 0;

            ReadBinaryRaster((char*)filepath.c_str(), &head, &buf, &bpi, &misw, &mish);

            if ((data8 = (char*)malloc(misw * mish * sizeof(char))) == NULL)
                syserr("show_mis", "", "unable to allocate 8 bit space");

            bits2bytes(buf, (u_char*)data8, misw * mish);

            for (dptr = data8, j = 0; j < 1; j++)
            {
                const auto width = misw;
                const auto height = mish;
                // Grayscale: one byte per pixel
                const auto bytesPerPixel = 1;
                // allocate memory
                auto image = new unsigned char[width * height * bytesPerPixel];

                for (int k = 0; k < mish; k++)
                {
                    for (int l = 0; l < misw; l++)
                    {
                        auto offset = (k * width + l) * bytesPerPixel;
                        // red and green fade from 0 to 255, blue is always 127
                        auto red = 255 * l / width;
                        auto green = 255 * k / height;
                        //image[offset] = (red + green) / 2;;
                        image[offset] = (*dptr++) ? 0 : 255;
                    }
                }

                // start JPEG compression
                // note: myOutput is the function defined in line 18, it saves the output in example.jpg
                // optional parameters:
                const bool isRGB = false; // true = RGB image, else false = grayscale
                const auto quality = 90;    // compression quality: 0 = worst, 100 = best, 80 to 90 are most often used
                const bool downsample = false; // false = save as YCbCr444 JPEG (better quality), true = YCbCr420 (smaller file)
                const char* comment = "TooJpeg example image"; // arbitrary JPEG comment

                std::ostringstream oss;
                oss << (char*)"temp.pct" << ".jpg";

                myFile = std::ofstream(oss.str(), std::ios_base::out | std::ios_base::binary);

                auto ok = TooJpeg::writeJpeg(myOutput, image, width, height, isRGB, quality, downsample, comment);
                delete[] image;
            }

            std::ifstream file("temp.pct.jpg", std::ios::in | std::ios::binary);
            if (!file) {
                std::cerr << "An error occurred opening the file\n";
                //return 12345;
            }
            file.seekg(0, std::ifstream::end);
            std::streampos jpeg_size_bytes = file.tellg();
            file.seekg(0);

            char* jpgbuffer = new char[jpeg_size_bytes];
            file.read(jpgbuffer, jpeg_size_bytes);

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
            hsfpage_row.hsf_num             = std::stoi(hsfdirname);
            hsfpage_row.ihead_id            = ihead_id;
            hsfpage_row.writer_num          = std::stoi(writer);
            hsfpage_row.template_num        = std::stoi(templ);
            hsfpage_row.buffer_len_bytes    = jpeg_size_bytes;
            hsfpage_row.buffer              = jpgbuffer;

            const int hsfpage_id = dbm.Insert(hsfpage_row);
            delete[] jpgbuffer;

            free(data8);
            free(head);

            break;
        }
        //break;
    }
}