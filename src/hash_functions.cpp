#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <nmmintrin.h>

#include "hash_functions.h"

size_t control_sum_hash(const char* str, size_t table_len)
{
    assert(str);

    size_t res = 0;

    for (size_t i = 0; str[i]; i++) res += (size_t) str[i];

    return res % table_len;
}

size_t control_sum_div_len(const char* str, size_t table_len)
{
    assert(str);

    size_t res = 0;

    for (size_t i = 0; str[i]; i++) res += (size_t) str[i];

    size_t len = strlen(str);

    return (res / len) % table_len;
}

size_t zero_hash(const char* str, size_t table_len)
{
    assert(str);

    return 0 * table_len;
}

size_t first_alpha_hash(const char* str, size_t table_len)
{
    assert(str);

    return ((size_t) str[0]) % table_len;
}

size_t len_hash(const char* str, size_t table_len)
{
    assert(str);

    size_t len = strlen(str);

    return len % table_len;
}

size_t ror_hash(const char* str, size_t table_len)
{
    assert(str);

    size_t temp_symbol = 0;

    for (size_t i = 0; str[i]; i++)
    {   
        temp_symbol = ((temp_symbol >> 1) | (temp_symbol << (sizeof(temp_symbol) * 8 - 1))) ^ ((size_t) str[i]);
    }

    return temp_symbol % table_len;
}

size_t crc32_hash(const char* str, size_t table_len) 
{
    assert(str);

    unsigned int crc = 0xFFFFFFFF;

    for (size_t i = 0; str[i]; i++)
    {
        unsigned int byte = (unsigned int) str[i];     
        crc = crc ^ byte;

        for (int j = 0; j < 8; j++) 
        {   
           unsigned int mask = -(crc & 1);
           crc = (crc >> 1) ^ (0x11EDC6F41 & mask);
       }
    }

    return (size_t) (~crc) % table_len;
}

size_t crc32_hash_simd(const char* str, size_t table_len)
{
    assert(str);

    unsigned int crc = 0xFFFFFFFF;

    for (size_t i = 0; str[i]; i++)
    {
        crc = _mm_crc32_u8(crc, (unsigned char) str[i]);
    }

    return (size_t) crc % table_len;
}