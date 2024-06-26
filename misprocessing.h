#pragma once
#ifndef MISPROCESSING_H_
#define MISPROCESSING_H_

#include <string>
#include <vector>

#include "sd19config.h"

struct MisInfo {
    std::string filepath = "";
    std::string filename = "";
    std::string clsfilename = "";
    std::string clsfilepath = "";
    std::string templ = "";
    std::string writer = "";
    std::string field_type = "";
    std::string hsf_num = "";
    std::string mis_sha256 = "";
    std::vector<std::string> mischars = std::vector<std::string>{};
};

std::vector<MisInfo> GetMisFiles();

void process_mis_thread_callback(const MisInfo info, const Sd19Config config);

std::ostream& operator<<(std::ostream& out, const MisInfo& ihead_row);

std::string to_string(const MisInfo& h);

#define MIS_BACKGROUND_PIXEL 0
#define MIS_FOREGROUND_PIXEL 255

#endif