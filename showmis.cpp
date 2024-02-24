#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "sqlite3/sqlite3.h" 

#include "toojpeg/toojpeg.h"

#include "sd19/bits2bytes.h"
#include "sd19/fatalerr.h"
#include "sd19/freemis.h"
#include "sd19/ihead.h"
#include "sd19/mis.h"
#include "sd19/readmis.h"
#include "sd19/readrast.h"
#include "sd19/syserr.h"

#include "sd19/allocmis.h"
#include "sd19/getnset.h"
#include "sd19/mfs.h"
#include "sd19/newext.h"
#include "sd19/normalize.h"
#include "sd19/shear.h"
#include "sd19/writemis.h"

#define ES  	  32
#define ES4     4*ES            /* always 4 ES          */

using namespace std;

void show_mis(char*);

std::ofstream myFile; 

int main()
{
	char misfile[] = "hsf_0.mis";
	//char misfile[] = "hsf_0.nrm";
	show_mis(misfile);
	return 0;
}
void myOutput(unsigned char byte)
{
	myFile << byte;
}

void mis_to_jpegs(char* misfile)
{

	//sqlite3* DB;
	//std::string sql = "CREATE TABLE PERSON("
	//	"ID INT PRIMARY KEY     NOT NULL, "
	//	"NAME           TEXT    NOT NULL, "
	//	"SURNAME          TEXT     NOT NULL, "
	//	"AGE            INT     NOT NULL, "
	//	"ADDRESS        CHAR(50), "
	//	"SALARY         REAL );";
	//int exit = 0;
	//exit = sqlite3_open("example.db", &DB);
	//char* messaggeError;
	//exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

	//if (exit != SQLITE_OK) {
	//	std::cerr << "Error Create Table" << std::endl;
	//	sqlite3_free(messaggeError);
	//}
	//else
	//	std::cout << "Table created Successfully" << std::endl;
	//sqlite3_close(DB);


	MIS* mis;
	char* data8, * dptr;
	int   j, k, l;

	mis = readmisfile(misfile);

	if (mis->misd != 1)
		fatalerr("show_mis", misfile, "incorrect entry size or depth");

	if ((data8 = (char*)malloc(mis->misw * mis->mish * sizeof(char))) == NULL)
		syserr("show_mis", misfile, "unable to allocate 8 bit space");

	////MFS* names;
	////int   i, n, progress;
	////MIS* inp, * out;
	//MIS* out;
	//float* sfctr, * scal_x, * scal_y;
	//
	//out = allocmis(NULL, 1, 0, NORM_W, NORM_H, mis->ent_num);

	////norm_2nd_gen2(&out, &scal_x, &scal_y, mis);
	//freemis(mis);
	////free(scal_x);
	////free(scal_y);
	//shear_mis(&mis, &sfctr, out);
	//freemis(out);
	//free(sfctr);
	//set_compression(mis->head, CCITT_G4);
	//newext(misfile, strlen(misfile), (char*)"nrm");
	//writemisfile(misfile, mis);

	//free(data8);
	//freemis(mis);

	//mis = readmisfile(misfile);

	//if (mis->misd != 1)
	//    fatalerr("show_mis", misfile, "incorrect entry size or depth");

	//if ((data8 = (char*)malloc(mis->misw * mis->mish * sizeof(char))) == NULL)
	//    syserr("show_mis", misfile, "unable to allocate 8 bit space");

	bits2bytes(mis->data, (u_char*)data8, mis->misw * mis->mish);

	for (dptr = data8, j = 0; j < mis->ent_num; j++)
	{
		// 800x600 image
		const auto width = mis->entw;
		const auto height = mis->enth;
		// Grayscale: one byte per pixel
		const auto bytesPerPixel = 1;
		// allocate memory
		auto image = new unsigned char[width * height * bytesPerPixel];

		for (k = 0; k < mis->enth; k++)
		{
			for (l = 0; l < mis->entw; l++)
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
		oss << misfile << "_" << j << ".jpg";
		//std::string var = oss.str();

		myFile = std::ofstream(oss.str(), std::ios_base::out | std::ios_base::binary);

		//auto ok = TooJpeg::writeJpeg(myOutput, image, width, height, isRGB, quality, downsample, comment);
		//delete[] image;
	}

	free(data8);
	freemis(mis);
}

void show_mis(char* misfile)
{
	mis_to_jpegs(misfile);

	//int   j, k, l;
	//MIS* mis;
	//char* data8, * dptr;

	//mis = readmisfile(misfile);

	//if (mis->misd != 1)
	//    fatalerr("show_mis", misfile, "incorrect entry size or depth");

	//if ((data8 = (char*)malloc(mis->misw * mis->mish * sizeof(char))) == NULL)
	//    syserr("show_mis", misfile, "unable to allocate 8 bit space");

	//if ((mis->misd != 1))
	//    fatalerr(misfile, "", "not binary data");
	//if (!((mis->enth == ES) && (mis->entw == ES)) &&
	//    !((mis->enth == ES4) && (mis->entw == ES4)))
	//    fatalerr(misfile, "", "not 32x32 nor 128x128 mis entries");

	//bits2bytes(mis->data, (u_char*)data8, mis->misw * mis->mish);

	//for (dptr = data8, j = 0; j < mis->ent_num; j++)
	//{
	//    for (k = 0; k < mis->enth; k++)
	//    {
	//        for (l = 0; l < mis->entw; l++)
	//        {
	//            fprintf(stdout, "%c%c", (*dptr++) ? '#' : '.', ((l + 1) % mis->misw) ? ' ' : '\n');
	//        }
	//    }
	//    fprintf(stdout, "\n");
	//}
	//free(data8);
	//freemis(mis);



	////MFS* names;
	////int   i, n, progress;
	//MIS* inp, * out;
	//float* sfctr, * scal_x, * scal_y;

	//inp = readmisfile(misfile);

	//if ((inp->misd != 1))
	//    fatalerr(misfile, misfile, "not binary data");
	//if (!((inp->enth == ES) && (inp->entw == ES)) &&
	//    !((inp->enth == ES4) && (inp->entw == ES4)))
	//    fatalerr(misfile, misfile,
	//        "not 32x32 nor 128x128 mis entries");

	//if (inp->entw == ES4)
	//{
	//    //if (progress == TRUE)
	//    //    fprintf(stdout, "doing %4d chars of %s\r",
	//    //        inp->ent_num, misfile), fflush(stdout);

	//    norm_2nd_gen2(&out, &scal_x, &scal_y, inp);
	//    freemis(inp);
	//    free(scal_x);
	//    free(scal_y);
	//    shear_mis(&inp, &sfctr, out);
	//    freemis(out);
	//    free(sfctr);
	//    set_compression(inp->head, CCITT_G4);
	//    newext(misfile, strlen(misfile), (char*)"nrm");
	//    writemisfile(misfile, inp);
	//}
	////if (progress == TRUE)
	////    fprintf(stdout, "\n");

	////hsf_0.nrm

	//inp = readmisfile((char*)"hsf_0.nrm");

	//if ((inp->misd != 1))
	//    fatalerr(misfile, misfile, "not binary data");
	//if (!((inp->enth == ES) && (inp->entw == ES)) &&
	//    !((inp->enth == ES4) && (inp->entw == ES4)))
	//    fatalerr(misfile, misfile,
	//        "not 32x32 nor 128x128 mis entries");

}

