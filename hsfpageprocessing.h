#pragma once
#ifndef HSFPAGEPROCESSING_H_
#define HSFPAGEPROCESSING_H_

#include <string>
#include <vector>

struct HsfPageInfo {
    std::string filepath = "";
    std::string filename = "";
    std::string templ = "";
    std::string hsfdirname = "";
    std::string writer = "";
    std::string hsfpage_sha256 = "";
};

std::vector<HsfPageInfo> GetHsfPages();

void process_hsf_page_thread_callback(const HsfPageInfo info);

std::ostream& operator<<(std::ostream& out, const HsfPageInfo& ihead_row);

std::string to_string(const HsfPageInfo& h);
#endif