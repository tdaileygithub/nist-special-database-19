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

// Connection event handler function
static void fn(struct mg_connection* c, int ev, void* ev_data) {
    if (ev == MG_EV_HTTP_MSG) {  // New HTTP request received
        struct mg_http_message* hm = (struct mg_http_message*)ev_data;  // Parsed HTTP request
        if (mg_http_match_uri(hm, "/api/hello")) {                        // REST API call?
            mg_http_reply(c, 200, "", "{%m:%d}\n", MG_ESC("status"), 1);    // Yes. Respond JSON
        }
        else {
            struct mg_http_serve_opts opts = { .root_dir = "." };  // For all other URLs,
            mg_http_serve_dir(c, hm, &opts);                     // Serve static files
        }
    }
}

int main()
{
    struct mg_mgr mgr;  // Mongoose event manager. Holds all connections
    mg_mgr_init(&mgr);  // Initialise event manager
    mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, NULL);  // Setup listener
    for (;;) {
        mg_mgr_poll(&mgr, 1000);  // Infinite event loop
    }
    return 0;

    std::chrono::steady_clock::time_point programstart = std::chrono::steady_clock::now();

    unsigned int NUM_THREADS = std::thread::hardware_concurrency() - 1;
    NUM_THREADS = (NUM_THREADS <= 0) ? 1 : NUM_THREADS;

    Sd19Config config;

    std::cout << setw(30) << "NIST_Special_Database_19: "   << setw(100) << config.GetNistSd19Folder() << std::endl;
    std::cout << setw(30) << "Sqlite_Db_Name: "             << setw(100) << config.GetNistSd19Folder() << std::endl;
    std::cout << setw(30) << "Hsf_Page_Enable: "            << setw(100) << (config.GetHfPageProcessing() ? "true" : "false") << std::endl;
    std::cout << setw(30) << "Delete_Existing_Db: "         << setw(100) << (config.GetDeleteExistingDb() ? "true" : "false") << std::endl;
    std::cout << setw(30) << "NumberThreads: "              << setw(100) << config.GetNumberThreads() << std::endl;
    std::cout << setw(30) << "Sqlite3_SourceDb: "           << setw(100) << config.GetSourceDbName() << std::endl;
    std::cout << setw(30) << "Sqlite3_DumpFilename: "       << setw(100) << config.GetDumpDbName() << std::endl;
    
    if (config.GetDeleteExistingDb())
    {        
        std::remove(config.GetSourceDbName().c_str());
        std::remove(config.GetDumpDbName().c_str());
    }
    dbm = std::make_unique< sdb19db::DbManager>(config.GetSourceDbName());

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
