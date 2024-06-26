#pragma once
#ifndef MNIST_FANN_EXTERN_H_
#define MNIST_FANN_EXTERN_H_

#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "fann/fann.h" 

extern std::map<char, int> counts_by_label;
extern std::vector<std::tuple<char, int>> train_dataset;
extern std::vector<std::tuple<char, int>> test_dataset;
extern std::map<char, int> label_to_index;

extern std::map<std::string, fann_activationfunc_enum> fann_activation_func_to_enum;
extern std::map<std::string, fann_train_enum> fann_training_algo_to_enum;

#define MIS_BACKGROUND_PIXEL 0
#define MIS_FOREGROUND_PIXEL 255

#endif