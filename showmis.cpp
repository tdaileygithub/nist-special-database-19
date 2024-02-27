#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

#include "pugixml/pugixml.hpp"
#include "ulog/ulog.h"

#include "sd19db/dbmanager.h"

#include "hsfpageprocessing.h"
#include "misprocessing.h"
#include "sd19config.h"

using namespace std;

//const char DB_NAME[] = "sd19.db3";
const char DB_NAME[] = ":memory:";

// hardcoded a global instance
std::unique_ptr<sdb19db::DbManager> dbm = nullptr;
// a global instance of std::mutex to protect global variable
std::mutex dbmutex;
std::mutex readbinaryrastermutex;

int main()
{
    unsigned int NUM_THREADS = std::thread::hardware_concurrency() - 1;
    NUM_THREADS = (NUM_THREADS <= 0) ? 1 : NUM_THREADS;

    Sd19Config config;

    std::cout << setw(30) << "NIST_Special_Database_19: "   << setw(100) << config.GetNistSd19Folder() << std::endl;
    std::cout << setw(30) << "Sqlite_Db_Name: "             << setw(100) << config.GetNistSd19Folder() << std::endl;
    std::cout << setw(30) << "Hsf_Page_Enable: "            << setw(100) << (config.GetHfPageProcessing() ? "true" : "false") << std::endl;
    std::cout << setw(30) << "Delete_Existing_Db: "         << setw(100) << (config.GetDeleteExistingDb() ? "true" : "false") << std::endl;
    std::cout << setw(30) << "NumberThreads: "              << setw(100) << config.GetNumberThreads() << std::endl;
    std::cout << setw(30) << "DbName: "                     << setw(100) << config.GetDbName() << std::endl;
    
    if (config.GetDeleteExistingDb())
    {
        std::remove(DB_NAME);
    }
    dbm = std::make_unique< sdb19db::DbManager>(config.GetDbName());

    using namespace sdb19db;

    if (config.GetHfPageProcessing())
    {
        std::vector<std::thread> vecOfThreads;
        auto hsfPagesToProcess = GetHsfPages();

        int numHsfPagesToProcess = hsfPagesToProcess.size() - 1;
        while (numHsfPagesToProcess != 0)
        {
            std::vector<std::thread> vecOfThreads;
            for (int i = 0; i < NUM_THREADS; i++)
            {
                std::thread threadObj(process_hsf_page_thread_callback, hsfPagesToProcess[numHsfPagesToProcess]);
                vecOfThreads.push_back(std::move(threadObj));

                numHsfPagesToProcess = numHsfPagesToProcess - 1;
                if (-1 == numHsfPagesToProcess) {
                    break;
                }
            };
            for (int j = 0; j < vecOfThreads.size(); j++) {
                vecOfThreads[j].join();                
            }
            if (-1 == numHsfPagesToProcess) {
                break;
            }
        }
    }
    if (config.GetMisProcessing())
    {
        std::vector<std::thread> vecOfThreads;
        auto misToProcess = GetMisFiles();

        int numMisFilesToProcess = misToProcess.size() - 1;
        while (numMisFilesToProcess != 0)
        {
            std::vector<std::thread> vecOfThreads;
            for (int i = 0; i < NUM_THREADS; i++)
            {
                std::thread threadObj(process_mis_thread_callback, misToProcess[numMisFilesToProcess]);
                vecOfThreads.push_back(std::move(threadObj));

                numMisFilesToProcess = numMisFilesToProcess - 1;
                if (-1 == numMisFilesToProcess) {
                    break;
                }
            };
            for (int j = 0; j < vecOfThreads.size(); j++) {
                vecOfThreads[j].join();
            }
            if (-1 == numMisFilesToProcess) {
                break;
            }            
        }
    }
    exit(0);	
}
