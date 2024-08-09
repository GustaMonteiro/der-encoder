#include "der_encoder.h"

enum TAG : unsigned char
{
    INTEGER = 0x02,
    SEQUENCE = 0x30
};

Der_Encoder &Der_Encoder::encode_integer(long long value)
{
    auto &current_sequence = this->sequences.top();

    current_sequence.push_back(INTEGER);

    int number_of_bytes = sizeof(value);
    bool is_negative = value < 0;
    bool leading_byte_added = false;

    std::vector<unsigned char> value_bytes;

    for (int i = sizeof(value) - 1; i >= 0; i--)
    {
        unsigned char byte = (value >> (8 * i)) & 0xFF;
        if (leading_byte_added || (!is_negative && byte != 0) || (is_negative && byte != 0xff) || i == 0)
        {
            value_bytes.push_back(byte);
            leading_byte_added = true;
        }
    }

    if (!is_negative && (value_bytes[0] & 0x80))
        value_bytes.insert(value_bytes.begin(), 0x00);

    this->encode_size(value_bytes.size());

    current_sequence.insert(current_sequence.end(), value_bytes.begin(), value_bytes.end());

    return *this;
}

Der_Encoder &Der_Encoder::encode_integer(const std::vector<unsigned char> &value)
{
    auto &current_sequence = this->sequences.top();

    current_sequence.push_back(INTEGER);

    size_t size = value.size();
    bool need_leading_zero = value[0] & 0x80;

    if (need_leading_zero)
        size++;

    this->encode_size(size);

    if (need_leading_zero)
        current_sequence.push_back(0);

    current_sequence.insert(current_sequence.end(), value.begin(), value.end());

    return *this;
}

Der_Encoder &Der_Encoder::start_sequence()
{
    this->sequences.push({});
    return *this;
}

Der_Encoder &Der_Encoder::end_sequence()
{
    assert(this->sequences.size() >= 2);

    auto ending_sequence = this->sequences.top();
    this->sequences.pop();

    auto &current_sequence = this->sequences.top();

    current_sequence.push_back(SEQUENCE);

    this->encode_size(ending_sequence.size());

    current_sequence.insert(current_sequence.end(), ending_sequence.begin(), ending_sequence.end());

    return *this;
}

std::vector<unsigned char> Der_Encoder::get_buffer()
{
    assert(this->sequences.size() == 1);
    return this->sequences.top();
}

Der_Encoder::Der_Encoder()
{
    this->sequences.push({});
}

void Der_Encoder::encode_size(size_t size)
{
    if (size > 127)
    {
        unsigned char bytes_needed = 0;
        size_t temp = size;

        do
        {
            bytes_needed++;
            temp /= 256;
        } while (temp >= 1);

        this->sequences.top().push_back(0x80 + bytes_needed);

        for (int i = bytes_needed - 1; i >= 0; i--)
            this->sequences.top().push_back((size >> (8 * i)) & 0xff);
    }
    else
        this->sequences.top().push_back(size & 0xff);
}