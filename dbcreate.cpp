#include <filesystem>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

#include "ulog/ulog.h"

#include "mongoose/mongoose.h"

#include "sd19db/dbmanager.h"

#include "hsfpageprocessing.h"
#include "misprocessing.h"
#include "sd19config.h"

using namespace std;

// global instance - only want one owner and wrapped with mutex
std::unique_ptr<sdb19db::DbManager> dbm = nullptr;

// global std::mutexes for sqlite3 writes and the nist group 4 decompression code
std::mutex dbmutex;
std::mutex readbinaryrastermutex;

int main()
{
    std::chrono::steady_clock::time_point programstart = std::chrono::steady_clock::now();

    unsigned int NUM_THREADS = std::thread::hardware_concurrency() - 1;
    NUM_THREADS = (NUM_THREADS <= 0) ? 1 : NUM_THREADS;

    Sd19Config config;
    std::cout << config;

    if (config.GetDeleteExistingDb())
    {        
        std::remove(config.GetSourceDbName().c_str());
        std::remove(config.GetDumpDbName().c_str());
    }
    dbm = std::make_unique< sdb19db::DbManager>(config.GetSourceDbName());

    std::filesystem::current_path(config.GetNistSd19Folder());

    using namespace sdb19db;

    if (config.GetHfPageProcessing())
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

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
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Hsf Page Processing Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;        
    }
    if (config.GetMisProcessing())
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        std::vector<std::thread> vecOfThreads;
        auto misToProcess = GetMisFiles();

        int numMisFilesToProcess = misToProcess.size() - 1;
        while (numMisFilesToProcess != 0)
        {
            std::vector<std::thread> vecOfThreads;
            for (int i = 0; i < NUM_THREADS; i++)
            {
                std::thread threadObj(process_mis_thread_callback, misToProcess[numMisFilesToProcess], config);
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
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Mis Processing Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
    }
    if (config.GetDeleteExistingDb()) {
        if (0 == (config.GetSourceDbName()).compare(":memory:")) {
            dbm->WriteInMemoryToDisk(config.GetDumpDbName());
        }
    }
    std::chrono::steady_clock::time_point programend = std::chrono::steady_clock::now();
    std::cout << "Program Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(programend - programstart).count() << "[ms]" << std::endl;
    
    cout << "Press Enter to Exit";
    cin.ignore();
    exit(0);
}
