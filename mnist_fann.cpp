#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "mnist_fann_extern.h"

std::map<char, int> counts_by_label = {};
std::map<char, std::vector<int>> dbids_by_label = {};
std::vector<std::tuple<char, int>> train_dataset = {};
std::vector<std::tuple<char, int>> test_dataset = {};
std::map<char, int> label_to_index = {}; 

std::map<std::string, fann_activationfunc_enum> fann_activation_func_to_enum = 
{
    {"FANN_LINEAR",                     fann_activationfunc_enum::FANN_LINEAR},
    {"FANN_THRESHOLD",                  fann_activationfunc_enum::FANN_THRESHOLD},
    {"FANN_THRESHOLD_SYMMETRIC",        fann_activationfunc_enum::FANN_THRESHOLD_SYMMETRIC},
    {"FANN_SIGMOID",                    fann_activationfunc_enum::FANN_SIGMOID},
    {"FANN_SIGMOID_STEPWISE",           fann_activationfunc_enum::FANN_SIGMOID_STEPWISE},
    {"FANN_SIGMOID_SYMMETRIC",          fann_activationfunc_enum::FANN_SIGMOID_SYMMETRIC},
    {"FANN_SIGMOID_SYMMETRIC_STEPWISE", fann_activationfunc_enum::FANN_SIGMOID_SYMMETRIC_STEPWISE},
    {"FANN_GAUSSIAN",                   fann_activationfunc_enum::FANN_GAUSSIAN},
    {"FANN_GAUSSIAN_SYMMETRIC",         fann_activationfunc_enum::FANN_GAUSSIAN_SYMMETRIC},
    {"FANN_GAUSSIAN_STEPWISE",          fann_activationfunc_enum::FANN_GAUSSIAN_STEPWISE},
    {"FANN_SIGMOID_SYMMETRIC_STEPWISE", fann_activationfunc_enum::FANN_SIGMOID_SYMMETRIC_STEPWISE},
    {"FANN_ELLIOT",                     fann_activationfunc_enum::FANN_ELLIOT},
    {"FANN_ELLIOT_SYMMETRIC",           fann_activationfunc_enum::FANN_ELLIOT_SYMMETRIC},
    {"FANN_LINEAR_PIECE",               fann_activationfunc_enum::FANN_LINEAR_PIECE},
    {"FANN_LINEAR_PIECE_SYMMETRIC",     fann_activationfunc_enum::FANN_LINEAR_PIECE_SYMMETRIC},
    {"FANN_SIN_SYMMETRIC",              fann_activationfunc_enum::FANN_SIN_SYMMETRIC},
    {"FANN_COS_SYMMETRIC",              fann_activationfunc_enum::FANN_COS_SYMMETRIC},
    {"FANN_SIN",                        fann_activationfunc_enum::FANN_SIN},
    {"FANN_COS",                        fann_activationfunc_enum::FANN_COS},
};

std::map<std::string, fann_train_enum> fann_training_algo_to_enum =
{
    {"FANN_TRAIN_INCREMENTAL",          fann_train_enum::FANN_TRAIN_INCREMENTAL},
    {"FANN_TRAIN_BATCH",                fann_train_enum::FANN_TRAIN_BATCH},
    {"FANN_TRAIN_RPROP",                fann_train_enum::FANN_TRAIN_RPROP},
    {"FANN_TRAIN_QUICKPROP",            fann_train_enum::FANN_TRAIN_QUICKPROP},
    {"FANN_TRAIN_SARPROP",              fann_train_enum::FANN_TRAIN_SARPROP}
};