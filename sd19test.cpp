#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "toojpeg/toojpeg.h"

#include "doctest/doctest.h"

#include "sqlite3/sqlite3.h"

#include "sd19db/sd19db.h"

int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }

TEST_CASE("testing the factorial function")
{
	CHECK(factorial(1) == 1);
	CHECK(factorial(2) == 2);
	CHECK(factorial(3) == 6);
	CHECK(factorial(10) == 3628800);
}

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

TEST_CASE("zz")
{
    using namespace sdb19db;
    tables::ihead data;
    data.id = 2;

    IHead tbl(std::string_view{"db.db3"});
    auto a = tbl.Insert(data);

    CHECK(a == 10);

}