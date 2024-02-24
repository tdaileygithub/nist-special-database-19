#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "toojpeg/toojpeg.h"

#include "doctest/doctest.h"

#include "sqlite_orm/sqlite_orm.h"

int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }

struct User {
	int id;
	std::string name;
	std::vector<char> hash;  //  binary format
};

TEST_CASE("testing the factorial function")
{
	CHECK(factorial(1) == 1);
	CHECK(factorial(2) == 2);
	CHECK(factorial(3) == 6);
	CHECK(factorial(10) == 3628800);
}

TEST_CASE("sqlite_orm open close")
{
    using std::cout;
    using std::endl;
    using namespace sqlite_orm;
    auto storage = make_storage("blob.sqlite",
        make_table("users",
            make_column("id", &User::id, primary_key()),
            make_column("name", &User::name),
            make_column("hash", &User::hash)));
    storage.sync_schema();
    storage.remove_all<User>();

    User alex{
        0,
        "Alex",
        {0x10, 0x20, 0x30, 0x40},
    };
    alex.id = storage.insert(alex);

    cout << "users count = " << storage.count<User>() << endl;

    cout << "alex = " << storage.dump(storage.get<User>(alex.id)) << endl;

    auto hash = storage.get<User>(alex.id).hash;
    
    CHECK(hash.size() == 4);
    CHECK(hash[0] == 0x10);
    CHECK(hash[1] == 0x20);
    CHECK(hash[2] == 0x30);
    CHECK(hash[3] == 0x40);
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
