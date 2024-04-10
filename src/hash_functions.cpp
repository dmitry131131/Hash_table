#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <nmmintrin.h>

#include "hash_functions.h"

static unsigned int reverse(unsigned int x);

size_t control_sum_hash(const char* str, size_t table_len)
{
    assert(str);

    size_t res = 0;

    for (size_t i = 0; str[i]; i++) res += (size_t) str[i];

    return res % table_len;
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
        unsigned int byte = str[i];     
        crc = crc ^ byte;

        for (int j = 7; j >= 0; j--) 
        {   
           unsigned int mask = -(crc & 1);
           crc = (crc >> 1) ^ (0x11EDC6F41 & mask);
       }
    }

    return (size_t) (~crc) % table_len;
}

static unsigned int reverse(unsigned int x) 
{
    x = ((x & 0x55555555) <<  1) | ((x >>  1) & 0x55555555);
    x = ((x & 0x33333333) <<  2) | ((x >>  2) & 0x33333333);
    x = ((x & 0x0F0F0F0F) <<  4) | ((x >>  4) & 0x0F0F0F0F);
    x = (x << 24) | ((x & 0xFF00) << 8) | ((x >> 8) & 0xFF00) | (x >> 24);

    return x;
}

size_t crc32a(const char* str, size_t table_len) 
{
    assert(str);

    unsigned int crc = 0xFFFFFFFF;
    for (size_t i = 0; str[i]; i++) 
    {
        unsigned int byte = str[i];          // Get next byte.
        byte = reverse(byte);                // 32-bit reversal.
        crc  = reverse(crc);
        
        long int temp1 = byte << 8;
        long int temp2 = crc  << 32;
        long int temp3 = temp1 ^ temp2;

        crc = temp3 % 0x11EDC6F41;
    }
    
    return reverse(crc) % table_len;
}

size_t crc32_hash_simd(const char* str, size_t table_len)
{
    assert(str);

    unsigned int crc = 0xFFFFFFFF;

    for (size_t i = 0; str[i]; i++)
    {
        unsigned int byte = str[i];     
    
        crc = _mm_crc32_u8(crc, str[i]);
    }

    return (size_t) crc % table_len;
}