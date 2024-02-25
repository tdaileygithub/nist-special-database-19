#include "toojpeg_helper.h"

#include "toojpeg.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>

namespace TooJpeg
{
    static std::ofstream temporary_jpeg_stream("temp.jpg", std::ios_base::out | std::ios_base::binary);
    
    void write_single_jpeg_byte(unsigned char byte) {
        temporary_jpeg_stream << byte;
    }

    bool save_jpeg(
        const std::string filename,
        unsigned char* image,
        const int width,
        const int height,
        const int bytesPerPixel,
        const bool isRGB,
        const int quality,
        const bool downsample,
        const std::string comment
    )
    {
        std::ostringstream oss;
        oss << filename;
        temporary_jpeg_stream = std::ofstream(oss.str(), std::ios_base::out | std::ios_base::binary);
        
        bool ret = TooJpeg::writeJpeg(write_single_jpeg_byte, image, width, height, isRGB, quality, downsample, comment.c_str());        

        temporary_jpeg_stream.close();

        return ret;        
    }


    void misdata_to_bwimage(char* src, unsigned char* dptr, int width, int height, int bytesPerPixel)
    {
        //const auto width = misw;
        //const auto height = mish;
        // Grayscale: one byte per pixel
        //const auto bytesPerPixel = 1;
        // allocate memory
        //auto image = new unsigned char[width * height * bytesPerPixel];

        for (int k = 0; k < height; k++)
        {
            for (int l = 0; l < width; l++)
            {
                auto offset = (k * width + l) * bytesPerPixel;
                // red and green fade from 0 to 255, blue is always 127
                auto red = 255 * l / width;
                auto green = 255 * k / height;                
                dptr[offset] = (*src++) ? 0 : 255;
            }
        }
    }
}