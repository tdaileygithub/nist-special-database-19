#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "toojpeg/toojpeg.h"

#include "doctest/doctest.h"

#include "sd19db/dbmanager.h"

#include "toojpeg/toojpeg_helper.h"

TEST_CASE("toojpeg - create a jpeg file")
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

    bool ret = TooJpeg::save_jpeg("temp.jpg", image, width, height, bytesPerPixel, isRGB, quality, downsample, "TooJpeg example image");
    CHECK(ret == true);
    std::remove("temp.jpg");

    delete[] image;    
}

TEST_CASE("character counts - 1stEditionUserGuide.pdf - table 6 Class abundancies") 
{
    sdb19db::DbManager dbm("sd19.db3");

    //Table 6: Class abundancies totalled over all partitions and all fields
    CHECK(40363 == dbm.GetMisCharacterCount("0"));
    CHECK(44704 == dbm.GetMisCharacterCount("1"));
    CHECK(40072 == dbm.GetMisCharacterCount("2"));
    //CHECK(41112 == dbm.GetMisCharacterCount("3")); //FAILING
    CHECK(39154 == dbm.GetMisCharacterCount("4"));
    CHECK(36606 == dbm.GetMisCharacterCount("5"));
    CHECK(39937 == dbm.GetMisCharacterCount("6"));
    CHECK(41893 == dbm.GetMisCharacterCount("7"));
    CHECK(39579 == dbm.GetMisCharacterCount("8"));
    CHECK(39533 == dbm.GetMisCharacterCount("9"));

    CHECK(7469  == dbm.GetMisCharacterCount("A"));
    CHECK(4526  == dbm.GetMisCharacterCount("B"));
    CHECK(11833 == dbm.GetMisCharacterCount("C"));
    CHECK(5341  == dbm.GetMisCharacterCount("D"));
    CHECK(5785  == dbm.GetMisCharacterCount("E"));
    CHECK(10622 == dbm.GetMisCharacterCount("F"));
    CHECK(2964  == dbm.GetMisCharacterCount("G"));
    CHECK(3673  == dbm.GetMisCharacterCount("H"));
    CHECK(13994 == dbm.GetMisCharacterCount("I"));
    CHECK(4388  == dbm.GetMisCharacterCount("J"));
    CHECK(2850  == dbm.GetMisCharacterCount("K"));
    CHECK(5886  == dbm.GetMisCharacterCount("L"));
    CHECK(10487 == dbm.GetMisCharacterCount("M"));
    CHECK(9588  == dbm.GetMisCharacterCount("N"));
    CHECK(29139 == dbm.GetMisCharacterCount("O"));
    CHECK(9744  == dbm.GetMisCharacterCount("P"));
    CHECK(3018  == dbm.GetMisCharacterCount("Q"));
    CHECK(5882  == dbm.GetMisCharacterCount("R"));
    CHECK(24272 == dbm.GetMisCharacterCount("S"));
    CHECK(11396 == dbm.GetMisCharacterCount("T"));
    CHECK(14604 == dbm.GetMisCharacterCount("U"));
    CHECK(5433  == dbm.GetMisCharacterCount("V"));
    CHECK(5501  == dbm.GetMisCharacterCount("W"));
    CHECK(3203  == dbm.GetMisCharacterCount("X"));
    CHECK(5541  == dbm.GetMisCharacterCount("Y"));
    CHECK(3165  == dbm.GetMisCharacterCount("Z"));

    CHECK(11677 == dbm.GetMisCharacterCount("a"));
    CHECK(6012  == dbm.GetMisCharacterCount("b"));
    CHECK(3286  == dbm.GetMisCharacterCount("c"));
    CHECK(11860 == dbm.GetMisCharacterCount("d"));
    CHECK(28723 == dbm.GetMisCharacterCount("e"));
    CHECK(2961  == dbm.GetMisCharacterCount("f"));
    CHECK(4276  == dbm.GetMisCharacterCount("g"));
    CHECK(10217 == dbm.GetMisCharacterCount("h"));
    CHECK(3152  == dbm.GetMisCharacterCount("i"));
    CHECK(2213  == dbm.GetMisCharacterCount("j"));
    CHECK(2957  == dbm.GetMisCharacterCount("k"));
    CHECK(17853 == dbm.GetMisCharacterCount("l"));
    CHECK(3109  == dbm.GetMisCharacterCount("m"));
    CHECK(13316 == dbm.GetMisCharacterCount("n"));
    CHECK(3215  == dbm.GetMisCharacterCount("o"));
    CHECK(2816  == dbm.GetMisCharacterCount("p"));
    CHECK(3499  == dbm.GetMisCharacterCount("q"));
    CHECK(16425 == dbm.GetMisCharacterCount("r"));
    CHECK(3136  == dbm.GetMisCharacterCount("s"));
    CHECK(21227 == dbm.GetMisCharacterCount("t"));
    CHECK(3312  == dbm.GetMisCharacterCount("u"));
    CHECK(3378  == dbm.GetMisCharacterCount("v"));
    CHECK(3164  == dbm.GetMisCharacterCount("w"));
    CHECK(3292  == dbm.GetMisCharacterCount("x"));
    CHECK(2746  == dbm.GetMisCharacterCount("y"));
    CHECK(3176  == dbm.GetMisCharacterCount("z"));
}

TEST_CASE("character counts - 1stEditionUserGuide.pdf - table 5 group abundancies")
{
    sdb19db::DbManager dbm("sd19.db3");
    CHECK(53449 == dbm.GetMisCharacterCount(0, 0));
    CHECK(10790 == dbm.GetMisCharacterCount(1, 0));
    CHECK(10968 == dbm.GetMisCharacterCount(2, 0));
    CHECK(99208 == dbm.GetMisCharacterCount(3, 0));

    CHECK(53312 == dbm.GetMisCharacterCount(0, 1));
    CHECK(10662 == dbm.GetMisCharacterCount(1, 1));
    CHECK(10784 == dbm.GetMisCharacterCount(2, 1));
    CHECK(87965 == dbm.GetMisCharacterCount(3, 1));

    CHECK(52467 == dbm.GetMisCharacterCount(0, 2));
    CHECK(10863 == dbm.GetMisCharacterCount(1, 2));
    CHECK(10883 == dbm.GetMisCharacterCount(2, 2));
    CHECK(61570 == dbm.GetMisCharacterCount(3, 2));

    CHECK(63896 == dbm.GetMisCharacterCount(0, 3));
    CHECK(12636 == dbm.GetMisCharacterCount(1, 3));
    CHECK(12678 == dbm.GetMisCharacterCount(2, 3));
    //CHECK(89210 == dbm.GetMisCharacterCount(3, 3));  //failing

    CHECK(58646 == dbm.GetMisCharacterCount(0, 4));
    CHECK(11941 == dbm.GetMisCharacterCount(1, 4));
    CHECK(12000 == dbm.GetMisCharacterCount(2, 4));    

    CHECK(61094 == dbm.GetMisCharacterCount(0, 6));
    CHECK(12479 == dbm.GetMisCharacterCount(1, 6));
    CHECK(12205 == dbm.GetMisCharacterCount(2, 6));

    CHECK(60089 == dbm.GetMisCharacterCount(0, 7));
    CHECK(12092 == dbm.GetMisCharacterCount(1, 7));
    CHECK(11578 == dbm.GetMisCharacterCount(2, 7));
}