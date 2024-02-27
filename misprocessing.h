#pragma once

#include <string>
#include <vector>

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

void process_mis_thread_callback(const MisInfo info);

std::ostream& operator<<(std::ostream& out, const MisInfo& ihead_row);

std::string to_string(const MisInfo& h);