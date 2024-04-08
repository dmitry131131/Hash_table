#include <stdio.h>
#include <assert.h>

#include "hash_functions.h"

size_t control_sum_hash(const char* str, size_t table_len)
{
    assert(str);

    size_t res = 0;

    for (size_t i = 0; str[i]; i++) res += (size_t) str[i];

    return res % table_len;
}