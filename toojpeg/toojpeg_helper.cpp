#include "toojpeg_helper.h"

#include "toojpeg.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace TooJpeg
{
    std::ofstream temporary_jpeg_stream("temp.jpg", std::ios_base::out | std::ios_base::binary);
    
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
        for (int k = 0; k < height; k++)
        {            
            for (int l = 0; l < width; l++)
            {
                auto offset = (k * width + l) * bytesPerPixel;
                dptr[offset] = (*src++) ? MIS_FOREGROUND_PIXEL : MIS_BACKGROUND_PIXEL;
            }
        }
    }
}