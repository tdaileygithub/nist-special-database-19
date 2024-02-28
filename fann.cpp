#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

#include "ulog/ulog.h"

#include "fann/fann.h"
#include <fstream>

using namespace std;

#define print(x) cout << x << endl;

/* global variables definitions */
string error_data_log = "error_data.dat";
string network_file = "network.nn";
string training_file = "training.data";

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
        print(" > Saving network... ");
        fann_save(ann, network_file.c_str());
    }
    print("Epoch " << epochs << ", current mse: " << fann_get_MSE(ann));
    return 0;
}

int main(int argc, char* argv[])
{
    /* header */
    float desired_error = 0.001f;
    int max_epochs = 500;
    int epochs_between_reports = 1;
    struct fann* ann;
    fann_train_enum learning_algorithm = FANN_TRAIN_BATCH; // Default learning_algorithm

    //./train --mse 0.001 --epoch 500 --train training.data --backprop-batch

    print("Training parameters:")
        print("   Desired error: " << desired_error)
        print("   Max epochs: " << max_epochs)
        if (network_file == "network.nn")
        {
            print("   Creating new network: network.nn")
                ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);
        }
        else
        {
            print("   Using network file: " << network_file)
                ann = fann_create_from_file(network_file.c_str());
        }


    if (learning_algorithm == FANN_TRAIN_BATCH)
        print("   Learning algorithm: Backpropagation - batch")
        if (learning_algorithm == FANN_TRAIN_INCREMENTAL)
            print("   Learning algorithm: Backpropagation - incremental")
            if (learning_algorithm == FANN_TRAIN_QUICKPROP)
                print("   Learning algorithm: Quickprop")
                print("")

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

    fann_save(ann, network_file.c_str());

    fann_destroy(ann);

    return 0;
}