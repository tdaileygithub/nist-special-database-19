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

    char* filebuffer = new char[size_bytes];

    file.read(filebuffer, size_bytes);

    sha.update((unsigned char*)filebuffer, size_bytes);
    
    std::array<uint8_t, 32> digest = sha.digest();

    delete[] filebuffer;

    return SHA256::toString(digest);
}