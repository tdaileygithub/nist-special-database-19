#include <algorithm>
#include <cstdlib> 
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "ulog/ulog.h"

#include "fann/fann.h"

//#include "imgui_util.h"

using namespace std;

/* global variables definitions */
string error_data_log   = "error_data.dat";
string network_file     = "network.nn";
string training_file    = "training.data";
string images_file      = "train-images-idx3-ubyte";
string labels_file      = "train-labels-idx1-ubyte";

/* network topology */
const unsigned int num_input = 28 * 28;
const unsigned int num_output = 10;
const unsigned int num_layers = 3;
const unsigned int num_neurons_hidden = 300;

static std::vector<float> x_data_epoch = {  };
static std::vector<float> y_data_mse = {  };

int FANN_API test_callback(struct fann* ann, struct fann_train_data* train,
    unsigned int max_epochs, unsigned int epochs_between_reports,
    float desired_error, unsigned int epochs)
{
    const float mse = fann_get_MSE(ann);
    ofstream outfile;
    outfile.open(error_data_log.c_str(), ios::app);
    outfile << mse << endl;
    outfile.close();

    if (epochs % 50 == 0)
    {
        std::cout << "Saving Network.... " << std::endl;
        fann_save(ann, network_file.c_str());
    }
    std::cout << setw(10) << "Epoch: " << setw(10) << epochs << setw(30) << "current mse: " << setw(10) << mse << "size " << x_data_epoch.size() << std::endl;

    x_data_epoch.push_back(epochs);
    y_data_mse.push_back(mse);

    return 0;
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

int main(int argc, char* argv[])
{
    //Imgui_SetupContext();

    float desired_error = 0.01f;
    int max_epochs = 500;
    int epochs_between_reports = 1;
    struct fann* ann;
    // Default learning_algorithm
    fann_train_enum learning_algorithm = FANN_TRAIN_BATCH; 

    //./fann --mse 0.01 --epoch 500 --train training.data --backprop-batch

    std::cout << setw(30) << "Training parameters: "    << setw(100) << ""                  << std::endl;
    std::cout << setw(30) << "Training File: "          << setw(100) << training_file       << std::endl;
    std::cout << setw(30) << "Desired error: "          << setw(100) << desired_error       << std::endl;
    std::cout << setw(30) << "Max epochs: "             << setw(100) << max_epochs          << std::endl;
    std::cout << setw(30) << "Using network file: "     << setw(100) << network_file        << std::endl;
    std::cout << setw(30) << "Learning algorithm: "     << setw(100) << learning_algorithm  << std::endl;

    ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);
    //ann = fann_create_from_file(network_file.c_str());

    // Activation functions
    fann_set_activation_function_hidden(ann, FANN_SIGMOID);
    fann_set_activation_function_output(ann, FANN_SIGMOID);

    fann_set_training_algorithm(ann, learning_algorithm);

    if (learning_algorithm != FANN_TRAIN_RPROP) {
        fann_set_learning_momentum(ann, 0.9);
        fann_set_learning_rate(ann, 1.3);
    }

    fann_set_callback(ann, *(test_callback));

    fann_train_on_file(ann, training_file.c_str(), max_epochs, epochs_between_reports, desired_error);

    //Imgui_SetupContext();

    //// Our state
    //bool show_demo_window = true;
    //bool show_another_window = false;
    ////ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //// Main loop
    //bool done = false;
    //while (!done)
    //{
    //    // Poll and handle messages (inputs, window resize, etc.)
    //    // See the WndProc() function below for our to dispatch events to the Win32 backend.
    //    MSG msg;
    //    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
    //    {
    //        ::TranslateMessage(&msg);
    //        ::DispatchMessage(&msg);
    //        if (msg.message == WM_QUIT)
    //            done = true;
    //    }
    //    if (done)
    //        break;

    //    // Start the Dear ImGui frame
    //    ImGui_ImplDX12_NewFrame();
    //    ImGui_ImplWin32_NewFrame();
    //    ImGui::NewFrame();

    //    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    //    if (show_demo_window)
    //        ImGui::ShowDemoWindow(&show_demo_window);

    //    int   bar_data[11] = {1,2,3,4,5,6,7,8,9,10,11};
    //    float x_data[11] = { 1,2,3,4,5,6,7,8,9,10,11 };
    //    float y_data[11] = { 1,2,3,4,5,6,7,8,9,10,11 };
    //    std::random_device rd;
    //    std::mt19937 g(rd());
    //    std::shuffle(std::begin(y_data), std::end(y_data),g);

    //    
    //    if (ImPlot::BeginPlot("My Plot")) {
    //        ImPlot::PlotBars("My Bar Plot", bar_data, 11);
    //        ImPlot::PlotLine("My Line Plot", x_data, y_data, 11);   
    //        ImPlot::EndPlot();
    //    }        
    //    Imgui_Render();
    //}

    //Imgui_Cleanup();

    std::cout << "Complete. Saving Network.... " << std::endl;
    fann_save(ann, network_file.c_str());

    fann_destroy(ann);

    //test the newly trained network
    for (int i = 0; i < 10; i++)
    {
        test_it();
    }
    exit(0);

    return 0;
}
