#include <algorithm>
#include <cassert> 
#include <cstdlib> 
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <syncstream>
#include <thread>
#include <vector>

#include "mnist_fann_extern.h"

#include "ulog/ulog.h"

#include "fann/fann.h"

#include "sd19db/dbmanager.h"

#include "sd19config.h"

using namespace std;

/* global variables definitions */
string error_data_log   = "error_data.dat";
string network_file     = "network.nn";
string training_file    = "training.data";
string images_file      = "train-images-idx3-ubyte";
string labels_file      = "train-labels-idx1-ubyte";

// global instance - only want one owner and wrapped with mutex
std::unique_ptr<sdb19db::DbManager> dbm = nullptr;

static unsigned int epoch = 0;
static float mse = 0.0f;

static std::chrono::steady_clock::time_point beginepoch = std::chrono::steady_clock::now();

int FANN_API test_callback(struct fann* ann, struct fann_train_data* train,
    unsigned int max_epochs, unsigned int epochs_between_reports,
    float desired_error, unsigned int epochs)
{
    epoch = epochs;
    mse = fann_get_MSE(ann);

    if (epochs % 10 == 0)
    {
        std::cout << "Saving Network.... " << std::endl;
        fann_save(ann, network_file.c_str());
    }
    std::chrono::steady_clock::time_point endepoch = std::chrono::steady_clock::now();
    std::cout << setw(10)   << "Epoch: "        << setw(10) << epochs       << setw(30) 
                            << "current mse: "  << setw(10) << mse          << setw(30);

    return 0;
}

void fann_get_train_data(unsigned int num, unsigned int num_input, unsigned int num_output, fann_type* input, fann_type* output)
{
    const auto t        = train_dataset.at(num);
    const char label    = std::get<0>(t);
    const int id        = std::get<1>(t);
    const int label_idx = label_to_index[label];

    auto rai = dbm->GetImageAsPng(id, "mis", "image");

    int ct = 0;
    for (int i = 0; i < rai.Height; i++)
    {
        for (int j = 0; j < rai.Width; j++)
        {
            const int offset = (rai.Width * i) + j;
            input[ct] = (float)(rai.PixelData[offset]) / 255.0f;
        }
        ct++;
    }       
    for (int i = 0; i < num_output; i++)
    {
        if (i == label_idx) {
            output[i] = 0.9f;
        }
        else {
            output[i] = 0.1f;
        }        
    }
    free(rai.PixelData);
}

void test_it()
{
    srand((unsigned)time(0)); 
    int item = (rand() % 55000) + 1;
    fann_type* calc_out;
    fann_type input[28 * 28];
    struct fann* ann;
    FILE* img_file;
    FILE* lbl_file;

    ann = fann_create_from_file(network_file.c_str());

    if (!(img_file = fopen(images_file.c_str(), "rb")))
    {
        exit(1);
    }
    if (!(lbl_file = fopen(labels_file.c_str(), "rb")))
    {
        exit(1);
    }

    fseek(img_file, 16, SEEK_SET);
    fseek(img_file, 28 * 28 * item, SEEK_CUR);

    fseek(lbl_file, 8, SEEK_SET);
    fseek(lbl_file, item, SEEK_CUR);

    unsigned char* lbl = new unsigned char;
    unsigned char* img = new unsigned char[28 * 28];
    fread(img, 1, 28 * 28, img_file);
    fread(lbl, 1, 1, lbl_file);

    int pixel;
    int label = *lbl;

    std::cout << "+----------------------------+" << std::endl;
    for (int i = 0; i < 28 * 28; i++)
    {
        pixel = img[i];
        input[i] = pixel;

        if (i % 28 == 0)
            std::cout << "|";
        if (pixel > 0)
            if (pixel > 127)
                std::cout << "X";
            else
                std::cout << ".";
        else
            std::cout << " ";
        if (i % 28 == 27)
            std::cout << "|" << std::endl;
    }
    std::cout << "+----------------------------+" << std::endl;

    calc_out = fann_run(ann, input);

    float out[2] = { -1, -1000 };
    for (int i = 0; i < 10; i++)
    {
        if (out[1] <= calc_out[i])
        {
            out[0] = i;
            out[1] = calc_out[i];
        }
        std::cout << "P(" << i << ") = " << calc_out[i] << "; " << std::endl;
    }
    std::cout << std::endl << std::endl;

    if (out[0] == label)
        std::cout << "  *** CORRECT ***";
    else
        std::cout << "  --- WRONG ---";
    std::cout << std::endl << std::endl;

    std::cout << 
        "Example number "   << item << std::endl <<
        "Expected  : "      << label << std::endl <<
        "Calculated: "      << out[0] << std::endl;

    fann_destroy(ann);

    delete[] img;
    delete lbl;

    fclose(img_file);
    fclose(lbl_file);
}

void train_thread(fann* ann, const Sd19Config& config)
{
    const unsigned int num_data = train_dataset.size();
    const unsigned int img_dim = config.GetMisScale() ? (32 * 32) : (128 * 128);
    const size_t num_bytes = static_cast<size_t>(img_dim) * num_data * sizeof(float);

    std::cerr << "num_data:   " << num_data << std::endl;
    std::cerr << "size in MB: " << (num_bytes) / (1000 * 1000)  << std::endl;

    fann_train_data* data = fann_create_train_from_callback(num_data, (unsigned int )config.GetNumberInputs(), (unsigned int)config.GetNumberOutputs(), fann_get_train_data);

    fann_train_on_data(ann, data, config.GetMaxEpochs(), config.GetEpochsBetweenReport(), config.GetDesiredError());
}

int main(int argc, char* argv[])
{
    Sd19Config config;
    std::cout << config << std::endl << std::endl;

    if (!std::filesystem::exists(config.GetSourceDbName())) {
        std::cerr << config.GetSourceDbName() << " not found" << std::endl;
        exit(1);
    }

    dbm = std::make_unique< sdb19db::DbManager>(config.GetSourceDbName());
    
    const float trainsplit = (config.GetTrainTestSplitPercent() / 100.0f);

    //Stratified training dataset    
    for (int asciichar = 48; asciichar <= 122; asciichar++)
    {
        //0-9
        if (asciichar >= 48 && asciichar <= 57)
        {
            label_to_index[(char)asciichar] = asciichar - 48;
        }
        //A-Z
        else if (asciichar >= 65 && asciichar <= 90)
        {
            label_to_index[(char)asciichar] = asciichar - 55;
        }
        //a-z
        else if (asciichar >= 97 && asciichar <= 122)
        {
            label_to_index[(char)asciichar] = asciichar - 61;
        }
        else 
        {
            continue;
        }
        //get all the mis database ids that are labeled with the asciichar
        const auto ids          = dbm->GetMisIds(std::string(1, (char)asciichar));
        counts_by_label[(char)asciichar] = ids.size();

        const auto trainbegin   = ids.cbegin();
        const auto trainend     = ids.cbegin() + (trainsplit * ids.size());
        const auto testbegin    = trainend;
        const auto testend      = ids.end();

        for (auto trainid = ids.cbegin(); trainid < trainend; trainid++) {
            train_dataset.push_back(std::make_tuple((char)asciichar, *trainid));
        }
        for (auto testid = testbegin; testid < testend; testid++) {
            test_dataset.push_back(std::make_tuple((char)asciichar, *testid));
        }
    }

    //ann = fann_create_from_file(network_file.c_str());
    struct fann* ann = fann_create_standard(config.GetNumberLayers(), config.GetNumberInputs(), config.GetNumberHidden(), config.GetNumberOutputs());

    // Activation functions
    fann_set_activation_function_hidden(ann, config.GetActionFunctionHiddenEnum());
    fann_set_activation_function_output(ann, config.GetActionFunctionOutputEnum());

    fann_set_training_algorithm(ann, config.GetTraningAlgorithmEnum());

    if (config.GetTraningAlgorithmEnum() != FANN_TRAIN_RPROP) {
        fann_set_learning_momentum(ann, config.GetLearningMomemtum());
        fann_set_learning_rate(ann, config.GetLearningRate());
    }

    fann_set_callback(ann, *(test_callback));

    thread thread_fann(train_thread, ann, config );
    thread_fann.join();

    std::cout << "Complete. Saving Network.... " << std::endl;
    fann_save(ann, network_file.c_str());

    fann_destroy(ann);

    //test the newly trained network
    for (int i = 0; i < 10; i++)
    {
        test_it();
    }
    return 0;
}
