#include "pch.h"

class Der_Encoder
{
public:
    Der_Encoder &encode_integer(long long value);
    Der_Encoder &encode_integer(const std::vector<unsigned char> &value);
    Der_Encoder &start_sequence();
    Der_Encoder &end_sequence();

    std::vector<unsigned char> get_buffer();

    Der_Encoder();

private:
    void encode_size(size_t size);

private:
    std::stack<std::vector<unsigned char>> sequences;
};
