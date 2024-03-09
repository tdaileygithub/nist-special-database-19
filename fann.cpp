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
#include <random>
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

std::unique_ptr<sdb19db::DbManager> dbm = nullptr;
std::unique_ptr<Sd19Config> config = nullptr;

int FANN_API test_callback(
    struct fann* ann, 
    struct fann_train_data* train,
    unsigned int max_epochs, 
    unsigned int epochs_between_reports,
    float desired_error, 
    unsigned int epochs)
{    
    const float mse = fann_get_MSE(ann);

    if (0 == (epochs % config->GetEpochsBetweenReport()))
    {
        std::cout << "Saving Network.... " << std::endl;
        fann_save(ann, config->GetNetworkFilename().c_str());

        std::cout << setw(10)   << "Epoch: "        << setw(10) << epochs       << setw(30) 
                                << "current mse: "  << setw(10) << mse          << setw(30) << std::endl;
    }
    return 0;
}

void fann_get_train_data(
    unsigned int num, 
    unsigned int num_input, 
    unsigned int num_output, 
    fann_type* input, 
    fann_type* output)
{
    const auto t        = train_dataset.at(num);
    const char label    = std::get<0>(t);
    const int id        = std::get<1>(t);
    const int label_idx = label_to_index[label];

    const auto rai = dbm->GetImageAsPng(id, "mis", "image");

    //std::cout << rai << std::endl;

    int ct = 0;
    for (int i = 0; i < rai.Height; i++)
    {
        for (int j = 0; j < rai.Width; j++)
        {
            const int offset = (rai.Width * i) + j;
            input[ct] = (rai.PixelData[offset]) ? (float)MIS_FOREGROUND_PIXEL : (float)MIS_BACKGROUND_PIXEL;
        }
        ct++;
    }
    for (int i = 0; i < num_output; i++) {
        output[i] = 0.0f;
    }
    output[label_idx] = 1.0f;
    free(rai.PixelData);
}

void test_it(const int idx)
{
    const auto testchar = test_dataset.at(idx);
    const char labelChar = std::get<0>(testchar);
    const int labelIndex = label_to_index[labelChar];

    const auto rai = dbm->GetImageAsPng(std::get<1>(testchar), "mis", "image");
    //std::cout << rai;

    fann_type* calc_out;
    fann_type input[32 * 32];
    struct fann* ann;

    ann = fann_create_from_file(config->GetNetworkFilename().c_str());

    for (int i = 0; i < rai.Height; i++)
    {
        for (int j = 0; j < rai.Width; j++)
        {
            int offset = rai.Width * i + j;
            input[offset] = (rai.PixelData[offset]) ? (float)MIS_FOREGROUND_PIXEL : (float)MIS_BACKGROUND_PIXEL;
        }
    }

    calc_out = fann_run(ann, input);

    float out[2] = { -1, -1000 };
    for (int i = 0; i < 62; i++)
    {
        //find max probability by class
        if (out[1] <= calc_out[i])
        {
            out[0] = i;
            out[1] = calc_out[i];
        }
        std::cout << "P(" << i << ") = " << calc_out[i] << "; " << std::endl;
    }
    std::cout << std::endl << std::endl;

    if (out[0] == labelIndex)
        std::cout << "  *** CORRECT ***";
    else
        std::cout << "  --- WRONG ---";

    std::cout << std::endl << std::endl;

    std::cout << 
        setw(20) << "MIS Id: "              << setw(10) << idx                          << std::endl <<
        setw(20) << "Expected Char:"        << setw(10) << labelChar                    << std::endl <<
        setw(20) << "Expected Index:"       << setw(10) << labelIndex                   << std::endl <<
        setw(20) << "Calculated Char: "     << setw(10) << label_to_index[(int)out[0]]  << std::endl <<
        setw(20) << "Calculated Index: "    << setw(10) << (int)out[0]                  << std::endl;

    fann_destroy(ann);
}

void train_thread(fann* ann, const Sd19Config* config)
{
    const unsigned int num_data = train_dataset.size() + test_dataset.size();
    const unsigned int img_dim = config->GetMisScale() ? (32 * 32) : (128 * 128);
    const size_t num_bytes = static_cast<size_t>(img_dim) * num_data * sizeof(float);

    std::cerr << "num_data:   " << num_data << std::endl;
    std::cerr << "size in MB: " << (num_bytes) / (1000 * 1000)  << std::endl;

    fann_train_data* data = fann_create_train_from_callback(train_dataset.size(), (unsigned int )config->GetNumberInputs(), (unsigned int)config->GetNumberOutputs(), fann_get_train_data);

    fann_train_on_data(ann, data, config->GetMaxEpochs(), config->GetEpochsBetweenReport(), config->GetDesiredError());
}

int main(int argc, char* argv[])
{
    std::random_device rd;
    std::mt19937 rng(rd());

    config = std::make_unique< Sd19Config>();
    std::cout << *config << std::endl << std::endl;
    
    dbm = std::make_unique< sdb19db::DbManager>(config->GetSourceDbName());
    
    const float trainsplit = (config->GetTrainTestSplitPercent() / 100.0f);
    const float testsplit =  1.0f - (config->GetTrainTestSplitPercent() / 100.0f);

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
    //shuffle the test + train vectors to avoid small sets only capturing from first few hsf pages or centrailized on a single char    
    std::shuffle(train_dataset.begin(), train_dataset.end(), rng);
    std::shuffle(test_dataset.begin(),  test_dataset.end(), rng);

    //train_dataset and test_dataset are already in the GetTrainTestSplitPercent() proportions

    //prune the train and test to have at most config->GetMaxDatasetSize() entries
    const int numTrainSplit = (int)(trainsplit * train_dataset.size());
    const int numTestSplit  = (int)(testsplit * test_dataset.size());
    const int numTotalDataSet = train_dataset.size() + test_dataset.size();

    if (numTotalDataSet > config->GetMaxDatasetSize())
    {        
        const int numDatasetToPrune = numTotalDataSet - config->GetMaxDatasetSize();
        const int numTrainSetToPrune = trainsplit * numDatasetToPrune;
        const int numTestSetToPrune = testsplit * numDatasetToPrune;

        train_dataset.erase(train_dataset.cbegin(), train_dataset.cbegin() + numTrainSetToPrune);
        test_dataset.erase(test_dataset.cbegin(), test_dataset.cbegin() + numTestSetToPrune);
    }

    std::cout << std::endl
        << std::setw(30) << std::string(30, '-')    << std::setw(85)    << std::string(85, '-')                         << std::endl
        << std::setw(30) << "GetMaxDatasetSize:"    << std::setw(85)    << config->GetMaxDatasetSize()                  << std::endl
        << std::setw(30) << "# Training MIS:"       << std::setw(85)    << train_dataset.size()                         << std::endl
        << std::setw(30) << "# Test MIS:"           << std::setw(85)    << test_dataset.size()                          << std::endl
        << std::setw(30) << "Total MIS:"            << std::setw(85)    << train_dataset.size() + test_dataset.size()   << std::endl
        << std::setw(30) << std::string(30, '-')    << std::setw(85)    << std::string(85, '-')                         << std::endl;

    struct fann* ann = fann_create_standard(config->GetNumberLayers(), config->GetNumberInputs(), config->GetNumberHidden(), config->GetNumberOutputs());

    fann_set_activation_function_hidden(ann, config->GetActionFunctionHiddenEnum());
    fann_set_activation_function_output(ann, config->GetActionFunctionOutputEnum());

    fann_set_training_algorithm(ann, config->GetTraningAlgorithmEnum());

    if (config->GetTraningAlgorithmEnum() != FANN_TRAIN_RPROP) {
        fann_set_learning_momentum(ann, config->GetLearningMomemtum());
        fann_set_learning_rate(ann, config->GetLearningRate());
    }

    fann_set_callback(ann, *(test_callback));

    thread thread_fann(train_thread, ann, config.get() );
    thread_fann.join();

    std::cout << "Complete. Saving Network.... " << std::endl;
    fann_save(ann, config->GetNetworkFilename().c_str());

    fann_destroy(ann);

    std::uniform_int_distribution<std::mt19937::result_type> testdist(0, test_dataset.size() - 1);

    //test the newly trained network
    for (int i = 0; i < 10; i++)
    {
        const int idx = testdist(rng);
        test_it(idx);
    }
    return 0;
}
