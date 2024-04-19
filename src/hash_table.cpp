#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include <nmmintrin.h>
#include <string.h>
#include <assert.h>

#include "hash_table.h"

static ssize_t find_elem_in_list_custom_inline(const ListData* list, const elem_t element);
static ssize_t find_elem_in_list_custom(const ListData* list, const elem_t element);

inline size_t crc32_hash_simd_1(const char* str, size_t table_len);

extern int strcmp_custom(const __m256i first, const __m256i second);

HashTableErrorCode hash_table_ctor(HashTable* hash_table, size_t capacity)
{
    assert(hash_table);

    if (capacity == 0) return WRONG_CAPACITY;

    hash_table->capacity = capacity;
    hash_table->table    = (ListData*) calloc(hash_table->capacity, sizeof(ListData));

    if (!hash_table->table) return ALLOC_MEMORY_ERROR;

    return NO_HASH_TABLE_ERROR;
}

HashTableErrorCode hash_table_dtor(HashTable* hash_table)
{
    assert(hash_table);

    for (size_t i = 0; i < hash_table->capacity; i++)
    {
        if (hash_table->table[i].capacity)            // Если создан список
        {
            if (list_dtor(&(hash_table->table[i]))) return LIST_DTOR_ERROR;
        }
    }

    free(hash_table->table);
    hash_table->capacity = 0;

    return NO_HASH_TABLE_ERROR;
}

HashTableErrorCode add_to_hash_table(HashTable* hash_table, char* str, size_t (*hash_function)(const char*, size_t))
{
    assert(hash_table);
    assert(str);

    size_t position = hash_function(str, hash_table->capacity);

    if (!(hash_table->table[position].capacity))
    {
        if (list_ctor(&(hash_table->table[position]), DEFAULT_LIST_SIZE)) return LIST_CTOR_ERROR;
    }

    if (!find_in_hash_table(hash_table, str, hash_function))
    {
        if (list_insert_in_tail(&(hash_table->table[position]), str)) return INSERT_IN_LIST_ERROR;
    }

    return NO_HASH_TABLE_ERROR;
}

char* find_in_hash_table(HashTable* hash_table, char* str, size_t (*hash_function)(const char*, size_t))
{
    assert(hash_table);
    assert(str);

    size_t position = hash_function(str, hash_table->capacity);

    if (!(hash_table->table[position].capacity)) return nullptr;

    ssize_t in_list_position = find_elem_in_list(&(hash_table->table[position]), str);

    return hash_table->table[position].data[in_list_position];
}

char* find_in_hash_table_inline(HashTable* hash_table, char* str)
{
    assert(hash_table);
    assert(str);

    size_t position = crc32_hash_simd(str, hash_table->capacity);

    if (!(hash_table->table[position].capacity)) return nullptr;

    ssize_t in_list_position = find_elem_in_list_custom_inline(&(hash_table->table[position]), str);

    return hash_table->table[position].data[in_list_position];
}

static ssize_t find_elem_in_list_custom_inline(const ListData* list, const elem_t element)
{
    assert(list);
    assert(element);

    ssize_t address  = list->next[list->head];
    size_t  list_len = list->len - 1;

    __m256i* element_data = (__m256i*) element;

    for (size_t i = 0; i < list_len; i++)
    {
        __m256i* str_data = (__m256i*) list->data[address];

        int out = 0;

        __asm__ (   "vmovdqa   %1, %%ymm0             \n\t"
                    "vmovdqa   %2, %%ymm1             \n\t"
                    "vpcmpeqb  %%ymm0, %%ymm1, %%ymm0 \n\t"
                    "vpmovmskb %%ymm0, %%eax          \n\t"
                    "notl      %%eax                  \n\t"
                    "movl      %%eax, %0              \n\t"
                :"=m"(out)
                :"m"(*element_data), "m"(*str_data)
                :"%eax"
                );

        if (!out) return address;

        address = list->next[address];
    }

    return 0;
}

static ssize_t find_elem_in_list_custom(const ListData* list, const elem_t element)
{
    assert(list);
    assert(element);

    ssize_t address  = list->next[list->head];
    size_t  list_len = list->len - 1;

    __m256i* element_data = (__m256i*) element;

    for (size_t i = 0; i < list_len; i++)
    {
        __m256i* str_data = (__m256i*) list->data[address];

        if (!strcmp_custom(*element_data, *str_data)) return address;

        address = list->next[address];
    }

    return 0;
}

/*
static int strcmp_custom(const __m256i* first, const __m256i* second)
{
    return ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(*first, *second));
}
*/


inline size_t crc32_hash_simd_1(const char* str, size_t table_len)
{
    assert(str);

    unsigned int crc = 0xFFFFFFFF;

    for (size_t i = 0; str[i]; i++)
    {
        crc = _mm_crc32_u8(crc, (unsigned char) str[i]);
    }

    return (size_t) crc % table_len;
}