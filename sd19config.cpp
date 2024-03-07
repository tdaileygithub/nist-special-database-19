#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "sd19config.h"

std::ostream& operator<<(std::ostream& out, const Sd19Config& config)
{
    out << std::endl
        << std::setw(30) << std::string(30, '-')            << std::setw(85) << std::string(85, '-')                << std::endl
        << std::setw(30) << "NIST 19 Folder:"               << std::setw(85) << config.GetNistSd19Folder()          << std::endl
        << std::setw(30) << "Sqlite3_SourceDb:"             << std::setw(85) << config.GetSourceDbName()            << std::endl
        << std::setw(30) << "Sqlite3_DumpFilename:"         << std::setw(85) << config.GetDumpDbName()              << std::endl
        << std::setw(30) << "Delete_Existing_Db:"           << std::setw(85) << config.GetDeleteExistingDb()        << std::endl
        << std::setw(30) << "Hsf_Page_Enable:"              << std::setw(85) << config.GetHfPageProcessing()        << std::endl
        << std::setw(30) << "Mis_Enable:"                   << std::setw(85) << config.GetMisProcessing()           << std::endl
        << std::setw(30) << "Scale_MIS:"                    << std::setw(85) << config.GetMisScale()                << std::endl
        << std::setw(30) << std::string(30, '-')            << std::setw(85) << std::string(85, '-')                << std::endl
        << std::setw(30) << "Number_Threads:"               << std::setw(85) << config.GetNumberThreads()           << std::endl        
        << std::setw(30) << std::string(30, '-')            << std::setw(85) << std::string(85, '-')                << std::endl        
        << std::setw(30) << "Train_Test_Split_Percent:"     << std::setw(85) << config.GetTrainTestSplitPercent()   << std::endl
        << std::setw(30) << "Maximum_Dataset_Size:"         << std::setw(85) << config.GetMaxDatasetSize()          << std::endl
        << std::setw(30) << "Training_Algorithm:"           << std::setw(85) << config.GetTraningAlgorithm()        << std::endl
        << std::setw(30) << "Training_Learning_Momentum:"   << std::setw(85) << config.GetLearningMomemtum()        << std::endl
        << std::setw(30) << "Training_Learning_Rate:"       << std::setw(85) << config.GetLearningRate()            << std::endl
        << std::setw(30) << std::string(30, '-')            << std::setw(85) << std::string(85, '-')                << std::endl
        << std::setw(30) << "Desired_Error:"                << std::setw(85) << config.GetDesiredError()            << std::endl
        << std::setw(30) << "Max_Epochs:"                   << std::setw(85) << config.GetMaxEpochs()               << std::endl
        << std::setw(30) << "Epochs_Between_Report:"        << std::setw(85) << config.GetEpochsBetweenReport()     << std::endl
        << std::setw(30) << std::string(30, '-')            << std::setw(85) << std::string(85, '-')                << std::endl
        << std::setw(30) << "Action_Function_Hidden:"       << std::setw(85) << config.GetActionFunctionHidden()    << std::endl
        << std::setw(30) << "Action_Function_Output:"       << std::setw(85) << config.GetActionFunctionOutput()    << std::endl
        << std::setw(30) << std::string(30, '-')            << std::setw(85) << std::string(85, '-')                << std::endl
        << std::setw(30) << "Number_Inputs:"                << std::setw(85) << config.GetNumberInputs()            << std::endl
        << std::setw(30) << "Number_Outputs:"               << std::setw(85) << config.GetNumberOutputs()           << std::endl
        << std::setw(30) << "Number_Layers:"                << std::setw(85) << config.GetNumberLayers()            << std::endl
        << std::setw(30) << "Number_Hidden:"                << std::setw(85) << config.GetNumberHidden()            << std::endl
        << std::setw(30) << std::string(30, '-')            << std::setw(85) << std::string(85, '-')                << std::endl;
    return out;
}

std::string to_string(const Sd19Config& config)
{
    std::ostringstream ss;
    ss << config;
    return ss.str();
}