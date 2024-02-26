#include <array>
#include <cstdint>
#include <fstream>
#include <string>

#include "sha256_util.h"

std::string get_file_sha256_checksum(const std::string filepath)
{
    SHA256 sha;
    
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    if (!file) {        
        exit(1);
    }
    file.seekg(0, std::ifstream::end);
    std::streampos size_bytes = file.tellg();
    file.seekg(0);

    char* hsfpagebuffer = new char[size_bytes];

    file.read(hsfpagebuffer, size_bytes);

    sha.update((unsigned char*)hsfpagebuffer, size_bytes);
    
    std::array<uint8_t, 32> digest = sha.digest();

    delete[] hsfpagebuffer;

    return SHA256::toString(digest);
}