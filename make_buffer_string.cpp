#include "make_buffer_string.h"

std::string make_buffer_string(const std::vector<unsigned char> &buffer)
{
    std::stringstream ss;

    for (auto byte : buffer)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;

    return ss.str();
}

void test_make_buffer_string()
{
    std::vector<unsigned char> buffer{1, 2, 3, 4, 5};

    assert(make_buffer_string(buffer) == "0102030405");
}