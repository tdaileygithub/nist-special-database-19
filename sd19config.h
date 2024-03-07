#pragma once
#ifndef SD19CONFIG_H_
#define SD19CONFIG_H_

#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "pugixml/pugixml.hpp"

#include "mnist_fann_extern.h"

#include "fann/fann.h"

#include "sd19db/dbmanager.h"

// global instance and mutex
extern std::unique_ptr<sdb19db::DbManager> dbm;
extern std::mutex dbmutex;
extern std::mutex readbinaryrastermutex;

class Sd19Config
{
private:
    std::string _nistSd19Folder = "";
    std::string _sourceDbFile = ":memory:";
    std::string _dbDumpFile = ":memory:";
    bool _hsfPageProcessing = false;
    bool _misProcessing = false;
    bool _deleteExistingDb = false;
    int _numberThreads = 1;
    int _numberInputs = 0;
    int _numberOutputs = 0;
    int _numberLayers = 0;
    int _numberHiddenLayers = 0;
    fann_activationfunc_enum _actionFunctionHidden;
    std::string _actionFunctionHiddenStr = "";
    fann_activationfunc_enum _actionFunctionOutput;
    std::string _actionFunctionOutputStr = "";
    fann_train_enum _trainingAlgorithm;
    std::string _trainingAlgorithmStr= "";
    float _learningMomentum = 0.0f;
    float _learningRate = 0.0f;
    float _desiredError = 0.0f;
    int _maxEpochs = 0;
    int _epochsBetweenReport = 0;
    bool _scaleMisCharacters = false;

public:
    Sd19Config() {
        pugi::xml_document doc;

        if (!doc.load_file("config.xml"))
        {
            std::cerr << "config.xml not found" << std::endl;
            exit(1);
        }
            

        _nistSd19Folder                 = std::string((pugi::xpath_query("/Sd19/Configs/Config[@Name='NIST_Special_Database_19']").evaluate_node_set(doc)[0]).node().text().get());
        _hsfPageProcessing              = (pugi::xpath_query("/Sd19/Configs/Config[@Name='Hsf_Page_Enable']").evaluate_node_set(doc)[0]).node().text().as_bool(true);
        _misProcessing                  = (pugi::xpath_query("/Sd19/Configs/Config[@Name='Mis_Enable']").evaluate_node_set(doc)[0]).node().text().as_bool(true);
        _deleteExistingDb               = (pugi::xpath_query("/Sd19/Configs/Config[@Name='Delete_Existing_Db']").evaluate_node_set(doc)[0]).node().text().as_bool(true);
        _numberThreads                  = (pugi::xpath_query("/Sd19/Configs/Config[@Name='Number_Threads']").evaluate_node_set(doc)[0]).node().text().as_int(0);
        _sourceDbFile                   = std::string((pugi::xpath_query("/Sd19/Configs/Config[@Name='Sqlite3_SourceDb']").evaluate_node_set(doc)[0]).node().text().get());
        _dbDumpFile                     = std::string((pugi::xpath_query("/Sd19/Configs/Config[@Name='Sqlite3_DumpFilename']").evaluate_node_set(doc)[0]).node().text().get());
        _scaleMisCharacters             = (pugi::xpath_query("/Sd19/Configs/Config[@Name='Scale_MIS']").evaluate_node_set(doc)[0]).node().text().as_bool(true);

        _numberInputs                   = (pugi::xpath_query("/Sd19/NeuralNetConfig/Config[@Name='Number_Inputs']").evaluate_node_set(doc)[0]).node().text().as_int(0);
        _numberOutputs                  = (pugi::xpath_query("/Sd19/NeuralNetConfig/Config[@Name='Number_Outputs']").evaluate_node_set(doc)[0]).node().text().as_int(0);
        _numberLayers                   = (pugi::xpath_query("/Sd19/NeuralNetConfig/Config[@Name='Number_Layers']").evaluate_node_set(doc)[0]).node().text().as_int(0);
        _numberHiddenLayers             = (pugi::xpath_query("/Sd19/NeuralNetConfig/Config[@Name='Number_Hidden']").evaluate_node_set(doc)[0]).node().text().as_int(0);

        _actionFunctionHiddenStr    = std::string((pugi::xpath_query("/Sd19/NeuralNetConfig/Config[@Name='Action_Function_Hidden']").evaluate_node_set(doc)[0]).node().text().get());
        _actionFunctionHidden       = fann_activation_func_to_enum[_actionFunctionHiddenStr];

        _actionFunctionOutputStr    = std::string((pugi::xpath_query("/Sd19/NeuralNetConfig/Config[@Name='Action_Function_Output']").evaluate_node_set(doc)[0]).node().text().get());
        _actionFunctionOutput       = fann_activation_func_to_enum[_actionFunctionOutputStr];

        _trainingAlgorithmStr       = std::string((pugi::xpath_query("/Sd19/NeuralNetConfig/Config[@Name='Training_Algorithm']").evaluate_node_set(doc)[0]).node().text().get());
        _trainingAlgorithm          = fann_training_algo_to_enum[_trainingAlgorithmStr];

        _learningMomentum           = (pugi::xpath_query("/Sd19/NeuralNetConfig/Config[@Name='Training_Learning_Momentum']").evaluate_node_set(doc)[0]).node().text().as_float(0);
        _learningRate               = (pugi::xpath_query("/Sd19/NeuralNetConfig/Config[@Name='Training_Learning_Rate']").evaluate_node_set(doc)[0]).node().text().as_float(0);

        _desiredError               = (pugi::xpath_query("/Sd19/NeuralNetConfig/Config[@Name='Desired_Error']").evaluate_node_set(doc)[0]).node().text().as_float(0);
        _maxEpochs                  = (pugi::xpath_query("/Sd19/NeuralNetConfig/Config[@Name='Max_Epochs']").evaluate_node_set(doc)[0]).node().text().as_int(0);
        _epochsBetweenReport        = (pugi::xpath_query("/Sd19/NeuralNetConfig/Config[@Name='Epochs_Between_Report']").evaluate_node_set(doc)[0]).node().text().as_int(0);        
    }

    std::string GetSourceDbName() const {
        return _sourceDbFile;
    }
    std::string GetDumpDbName() const {
        return _dbDumpFile;
    }
    std::string GetNistSd19Folder() const {
        return _nistSd19Folder;
    }
    bool GetHfPageProcessing() const {
        return _hsfPageProcessing;
    }
    bool GetDeleteExistingDb() const {
        return _deleteExistingDb;
    }
    bool GetMisProcessing() const {
        return _misProcessing;
    }
    int GetNumberThreads() const {
        unsigned int NUM_THREADS = std::thread::hardware_concurrency() - 1;
        if (_numberThreads == 0 || _numberThreads < 0) {
            return NUM_THREADS;
        }
        return _numberThreads;
    }
    bool GetMisScale() const {
        return _scaleMisCharacters;
    }

    int GetNumberInputs() const {
        return _numberInputs;
    }
    int GetNumberOutputs() const {
        return _numberOutputs;
    }
    int GetNumberHidden() const {
        return _numberHiddenLayers;
    }
    int GetNumberLayers() const {
        return _numberLayers;
    }

    fann_activationfunc_enum GetActionFunctionHiddenEnum() const {
        return _actionFunctionHidden;
    }
    std::string GetActionFunctionHidden() const {
        return _actionFunctionHiddenStr;
    }
    fann_activationfunc_enum GetActionFunctionOutputEnum() const {
        return _actionFunctionOutput;
    }
    std::string GetActionFunctionOutput() const {
        return _actionFunctionOutputStr;
    }
    std::string GetTraningAlgorithm() const {
        return _trainingAlgorithmStr;
    }
    fann_train_enum GetTraningAlgorithmEnum() const {
        return _trainingAlgorithm;
    }

    float GetLearningMomemtum() const {
        return _learningMomentum;
    }
    float GetLearningRate() const {
        return _learningRate;
    } 

    float GetDesiredError() const {
        return _desiredError;
    }
    int GetMaxEpochs() const {
        return _maxEpochs;
    }
    int GetEpochsBetweenReport() const {
        return _epochsBetweenReport;
    }
};

std::ostream& operator<<(std::ostream& out, const Sd19Config& config);

std::string to_string(const Sd19Config& config);
#endif