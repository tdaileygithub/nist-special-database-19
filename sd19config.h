#pragma once

#include <string>

#include "pugixml/pugixml.hpp"

// a global instance and mutex
//extern sdb19db::DbManager dbm;
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
    int  _numberThreads = 1;

public:
    Sd19Config() {
        pugi::xml_document doc;

        if (!doc.load_file("config.xml"))
            exit(1);

        _nistSd19Folder     = std::string((pugi::xpath_query("/Sd19Db/Configs/Config[@Name='NIST_Special_Database_19']").evaluate_node_set(doc)[0]).node().text().get());
        _hsfPageProcessing  = (pugi::xpath_query("/Sd19Db/Configs/Config[@Name='Hsf_Page_Enable']").evaluate_node_set(doc)[0]).node().text().as_bool(true);
        _misProcessing      = (pugi::xpath_query("/Sd19Db/Configs/Config[@Name='Mis_Enable']").evaluate_node_set(doc)[0]).node().text().as_bool(true);
        _deleteExistingDb   = (pugi::xpath_query("/Sd19Db/Configs/Config[@Name='Delete_Existing_Db']").evaluate_node_set(doc)[0]).node().text().as_bool(true);
        _numberThreads      = (pugi::xpath_query("/Sd19Db/Configs/Config[@Name='Number_Threads']").evaluate_node_set(doc)[0]).node().text().as_int(0);
        _sourceDbFile       = std::string((pugi::xpath_query("/Sd19Db/Configs/Config[@Name='Sqlite3_SourceDb']").evaluate_node_set(doc)[0]).node().text().get());
        _dbDumpFile         = std::string((pugi::xpath_query("/Sd19Db/Configs/Config[@Name='Sqlite3_DumpFilename']").evaluate_node_set(doc)[0]).node().text().get());
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
};