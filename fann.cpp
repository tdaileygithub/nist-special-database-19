#include <algorithm>
#include <cstdlib> 
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "ulog/ulog.h"

#include "fann/fann.h"

#include "imgui_util.h"

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

int FANN_API test_callback(struct fann* ann, struct fann_train_data* train,
    unsigned int max_epochs, unsigned int epochs_between_reports,
    float desired_error, unsigned int epochs)
{
    ofstream outfile;
    outfile.open(error_data_log.c_str(), ios::app);
    outfile << fann_get_MSE(ann) << endl;
    outfile.close();

    if (epochs % 50 == 0)
    {
        std::cout << "Saving Network.... " << std::endl;
        fann_save(ann, network_file.c_str());
    }
    std::cout << setw(10) << "Epoch: " << setw(10) << epochs << setw(30) << "current mse: " << setw(10) << fann_get_MSE(ann) << std::endl;
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
    Imgui_SetupContext();

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplDX12_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        int   bar_data[11] = {1,2,3,4,5,6,7,8,9,10,11};
        float x_data[11] = { 1,2,3,4,5,6,7,8,9,10,11 };
        float y_data[11] = { 1,2,3,4,5,6,7,8,9,10,11 };
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(std::begin(y_data), std::end(y_data),g);

        
        if (ImPlot::BeginPlot("My Plot")) {
            ImPlot::PlotBars("My Bar Plot", bar_data, 11);
            ImPlot::PlotLine("My Line Plot", x_data, y_data, 11);   
            ImPlot::EndPlot();
        }        

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
            
            ImGuiIO& io = ImGui::GetIO(); 
            (void)io;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
        Imgui_Render();
    }
    Imgui_Cleanup();

    return 0;
}
