#pragma once

#include <fstream>
#include <string>

namespace TooJpeg
{
    extern std::ofstream temporary_jpeg_stream;

    void write_single_jpeg_byte(unsigned char byte);

    void misdata_to_bwimage(unsigned char* src, unsigned char* dptr, int width, int height, int bytesPerPixel);

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
    );
}