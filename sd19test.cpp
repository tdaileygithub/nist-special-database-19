#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <cassert>
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

#include "sd19/bits2bytes.h"
#include "sd19/getnset.h"
#include "sd19/readmis.h"
#include "sd19/readrast.h"
#include "sd19/syserr.h"

#include "sd19db/dbmanager.h"
#include "sd19/fatalerr.h"

// output file
std::ofstream myFile("example.jpg", std::ios_base::out | std::ios_base::binary);

// write a single byte compressed by tooJpeg
void myOutput(unsigned char byte)
{
    myFile << byte;
}

//TEST_CASE("toojpeg create file")
//{
//    // 800x600 image
//    const auto width = 800;
//    const auto height = 600;
//    // RGB: one byte each for red, green, blue
//    const auto bytesPerPixel = 3;
//    // allocate memory
//    auto image = new unsigned char[width * height * bytesPerPixel];
//    // create a nice color transition (replace with your code)
//    for (auto y = 0; y < height; y++)
//        for (auto x = 0; x < width; x++)
//        {
//            // memory location of current pixel
//            auto offset = (y * width + x) * bytesPerPixel;
//            // red and green fade from 0 to 255, blue is always 127
//            image[offset] = 255 * x / width;
//            image[offset + 1] = 255 * y / height;
//            image[offset + 2] = 127;
//        }
//    // start JPEG compression
//    // note: myOutput is the function defined in line 18, it saves the output in example.jpg
//    // optional parameters:
//    const bool isRGB = true;  // true = RGB image, else false = grayscale
//    const auto quality = 90;    // compression quality: 0 = worst, 100 = best, 80 to 90 are most often used
//    const bool downsample = false; // false = save as YCbCr444 JPEG (better quality), true = YCbCr420 (smaller file)
//    const char* comment = "TooJpeg example image"; // arbitrary JPEG comment
//    auto ok = TooJpeg::writeJpeg(myOutput, image, width, height, isRGB, quality, downsample, comment);
//    CHECK(ok == true);
//    delete[] image
//    myFile.close();
//}

//TEST_CASE("ihead and hsfpage and mis - can insert 100 rows")
//{
//    srand(time(NULL));
//
//    using namespace sdb19db;
//
//    std::remove("db.db3");
//
//    DbManager dbm("db.db3");
//    dbm.Setup();
//
//    for (int i = 0; i < 100; i++)
//    {
//        tables::ihead ihead_row;
//        ihead_row.created    = "hello";
//        ihead_row.par_x      = 2;
//        ihead_row.parent     = "asdfadsf";
//        
//        const int ihead_id = dbm.Insert(ihead_row);        
//        CHECK((i+1) == ihead_id);
//        
//        tables::hsfpage hsfpage_row;
//        hsfpage_row.hsf_num             = rand() % 9;
//        hsfpage_row.ihead_id            = ihead_id;
//        hsfpage_row.writer_num          = rand() % 4170;
//        hsfpage_row.template_num        = rand() % 100;        
//        hsfpage_row.buffer_len_bytes    = (i + 1);        
//        hsfpage_row.buffer              = new char[hsfpage_row.buffer_len_bytes];
//        std::fill(hsfpage_row.buffer, hsfpage_row.buffer + hsfpage_row.buffer_len_bytes, (i+1));
//
//        const int hsfpage_id = dbm.Insert(hsfpage_row);
//        CHECK((i + 1) == hsfpage_id);
//
//        delete[] hsfpage_row.buffer;
//
//        tables::mis mis_row;
//        mis_row.hsf_num         = rand() % 9;
//        mis_row.ihead_id        = ihead_id;
//        mis_row.writer_num      = rand() % 4170;
//        mis_row.template_num    = rand() % 100;
//        mis_row.character       = char(rand() % 26 + 65);
//        mis_row.jpeg_len_bytes  = (i + 1);
//        mis_row.jpeg            = new char[mis_row.jpeg_len_bytes];
//        std::fill(mis_row.jpeg, mis_row.jpeg + mis_row.jpeg_len_bytes, (i + 1));
//
//        const int mis_id = dbm.Insert(mis_row);
//        CHECK((i + 1) == mis_id);
//
//        delete[] mis_row.jpeg;
//    }
//}

//TEST_CASE("ihead and hsfpage - insert and read")
//{
//    using namespace sdb19db;
//
//    std::remove("db.db3");
//    DbManager dbm("db.db3");
//
//    dbm.Setup();
//
//    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
//
//    for (const auto& dirEntry : recursive_directory_iterator("hsf_page"))
//    {
//        if (!dirEntry.is_directory())
//        {
//            //f0000_14.pct
//            std::string hsfdirname(dirEntry.path().parent_path().filename().string());
//            hsfdirname = std::regex_replace(hsfdirname, std::regex("hsf_page"), "");
//            hsfdirname = std::regex_replace(hsfdirname, std::regex("hsf_"), "");
//
//            const std::string filepath(dirEntry.path().string());
//            const std::string filename(dirEntry.path().filename().string());
//            const std::string writer(filename.substr(1, 4));
//            const std::string templ(filename.substr(6, 2));
//
//            std::cout 
//                << " processing: "  << filepath 
//                << " writer: "      << writer
//                << " template: "    << templ
//                << " dirname: "     << hsfdirname
//                << std::endl;
//
//            IHEAD* head;
//            unsigned char* buf;
//            int misw, mish, bpi, entw, enth, ent_num;
//            char* data8, * dptr;
//            int j = 0;
//
//            ReadBinaryRaster((char*)filepath.c_str(), &head, &buf, &bpi, &misw, &mish);
//
//            if ((data8 = (char*)malloc(misw * mish * sizeof(char))) == NULL)
//                syserr("show_mis", "", "unable to allocate 8 bit space");
//
//            bits2bytes(buf, (u_char*)data8, misw * mish);
//
//            for (dptr = data8, j = 0; j < 1; j++)
//            {
//                const auto width = misw;
//                const auto height = mish;
//                // Grayscale: one byte per pixel
//                const auto bytesPerPixel = 1;
//                // allocate memory
//                auto image = new unsigned char[width * height * bytesPerPixel];
//
//                for (int k = 0; k < mish; k++)
//                {
//                    for (int l = 0; l < misw; l++)
//                    {
//                        auto offset = (k * width + l) * bytesPerPixel;
//                        // red and green fade from 0 to 255, blue is always 127
//                        auto red = 255 * l / width;
//                        auto green = 255 * k / height;
//                        //image[offset] = (red + green) / 2;;
//                        image[offset] = (*dptr++) ? 0 : 255;
//                    }
//                }
//
//                // start JPEG compression
//                // note: myOutput is the function defined in line 18, it saves the output in example.jpg
//                // optional parameters:
//                const bool isRGB = false; // true = RGB image, else false = grayscale
//                const auto quality = 90;    // compression quality: 0 = worst, 100 = best, 80 to 90 are most often used
//                const bool downsample = false; // false = save as YCbCr444 JPEG (better quality), true = YCbCr420 (smaller file)
//                const char* comment = "TooJpeg example image"; // arbitrary JPEG comment
//
//                std::ostringstream oss;
//                oss << (char*)"temp.pct" << ".jpg";
//
//                myFile = std::ofstream(oss.str(), std::ios_base::out | std::ios_base::binary);
//
//                auto ok = TooJpeg::writeJpeg(myOutput, image, width, height, isRGB, quality, downsample, comment);
//                delete[] image
//                myFile.close();
//            }
//
//            std::ifstream file("temp.pct.jpg", std::ios::in | std::ios::binary);
//            if (!file) {
//                std::cerr << "An error occurred opening the file\n";
//                //return 12345;
//            }
//            file.seekg(0, std::ifstream::end);
//            std::streampos jpeg_size_bytes = file.tellg();
//            file.seekg(0);
//
//            char* jpgbuffer = new char[jpeg_size_bytes];
//            file.read(jpgbuffer, jpeg_size_bytes);
//
//            tables::ihead ihead_row;
//            ihead_row.created       = get_created(head);
//            ihead_row.width         = get_width(head);
//            ihead_row.height        = get_height(head);
//            ihead_row.depth         = get_depth(head);
//            ihead_row.density       = get_density(head);
//            ihead_row.compress      = get_compression(head);
//            ihead_row.complen       = get_complen(head);
//            ihead_row.align         = get_align(head);
//            ihead_row.unitsize      = get_unitsize(head);
//            ihead_row.sigbit        = get_sigbit(head);
//            ihead_row.byte_order    = get_byte_order(head);
//            ihead_row.pix_offset    = get_pix_offset(head);
//            ihead_row.whitepix      = get_whitepix(head);
//            ihead_row.issigned      = get_issigned(head);
//            ihead_row.rm_cm         = get_rm_cm(head);
//            ihead_row.tb_bt         = get_tb_bt(head);
//            ihead_row.lr_rl         = get_lr_rl(head);
//            ihead_row.parent        = get_parent(head);
//            ihead_row.par_x         = get_par_x(head);
//            ihead_row.par_y         = get_par_y(head);
//
//            const int ihead_id = dbm.Insert(ihead_row);            
//
//            tables::hsfpage hsfpage_row;
//            hsfpage_row.hsf_num             = std::stoi(hsfdirname);
//            hsfpage_row.ihead_id            = ihead_id;
//            hsfpage_row.writer_num          = std::stoi(writer);
//            hsfpage_row.template_num        = std::stoi(templ);
//            hsfpage_row.buffer_len_bytes    = jpeg_size_bytes;
//            hsfpage_row.buffer              = jpgbuffer;
//
//            const int hsfpage_id = dbm.Insert(hsfpage_row);
//            delete[] jpgbuffer;
//
//            free(data8);
//            free(head);
//
//            break;
//        }
//        //break;
//    }
//}


TEST_CASE("ihead and mis - insert and read")
{
    using namespace sdb19db;

    std::remove("db.db3");
    DbManager dbm("db.db3");

    dbm.Setup();

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

            if (mis->misd != 1)
                fatalerr("show_mis", "", "incorrect entry size or depth");

            if ((data8 = (char*)malloc(mis->misw * mis->mish * sizeof(char))) == NULL)
                syserr("show_mis", "", "unable to allocate 8 bit space");

            std::cout 
                << " processing: "  << filepath 
                << " writer: "      << writer
                << " template: "    << templ
                << " hsf_num: "     << hsf_num
                << " clsfilepath: " << clsfilepath
                << " characters: "  << mischars.size()
                << std::endl;

            bits2bytes(mis->data, (u_char*)data8, mis->misw * mis->mish);

            for (dptr = data8, misentry = 0; misentry < mis->ent_num; misentry++)
            {
                //std::cout << misentry << " character: " << mischars.at(misentry) << std::endl;
                //continue;
                {
                    const auto width = mis->entw;
                    const auto height = mis->enth;
                    // Grayscale: one byte per pixel
                    const auto bytesPerPixel = 1;
                    // allocate memory
                    auto image = new unsigned char[width * height * bytesPerPixel];

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

                    // start JPEG compression
                    // note: myOutput is the function defined in line 18, it saves the output in example.jpg
                    // optional parameters:
                    const bool isRGB = false; // true = RGB image, else false = grayscale
                    const auto quality = 90;    // compression quality: 0 = worst, 100 = best, 80 to 90 are most often used
                    const bool downsample = false; // false = save as YCbCr444 JPEG (better quality), true = YCbCr420 (smaller file)
                    const char* comment = "TooJpeg example image"; // arbitrary JPEG comment

                    std::ostringstream oss;
                    oss << "tempmis.jpg";

                    myFile = std::ofstream(oss.str(), std::ios_base::out | std::ios_base::binary);

                    auto ok = TooJpeg::writeJpeg(myOutput, image, width, height, isRGB, quality, downsample, comment);                    
                    delete[] image;
                    myFile.close();
                }
                
                {
                    std::ifstream misfile("tempmis.jpg", std::ios::in | std::ios::binary);
                    if (!misfile) {
                        std::cerr << "An error occurred opening the file\n";                
                    }
                    misfile.seekg(0, std::ifstream::end);
                    std::streampos jpeg_size_bytes = misfile.tellg();
                    misfile.seekg(0);

                    char* jpgbuffer = new char[jpeg_size_bytes];
                    misfile.read(jpgbuffer, jpeg_size_bytes);

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
                    mis_row.hsf_num             = std::stoi(hsf_num);
                    mis_row.ihead_id            = ihead_id;
                    mis_row.writer_num          = std::stoi(writer);
                    mis_row.template_num        = std::stoi(templ);
                    mis_row.character           = mischars.at(misentry);
                    mis_row.jpeg_len_bytes      = jpeg_size_bytes;
                    mis_row.jpeg                = jpgbuffer;

                    const int mis_id            = dbm.Insert(mis_row);
                    delete[] jpgbuffer;
                }
                //exit(1);
            }

            free(data8);
            free(mis);

            continue;

            //free(data8);
            //free(head);

            break;
        }
        //break;
    }
}