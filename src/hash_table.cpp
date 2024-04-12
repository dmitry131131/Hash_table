#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include <nmmintrin.h>
#include <string.h>
#include <assert.h>

#include "hash_table.h"

static ssize_t find_elem_in_list_custom(const ListData* list, const elem_t element);
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

    ssize_t in_list_position = find_elem_in_list_custom(&(hash_table->table[position]), str);

    if (in_list_position == -1) return nullptr;

    return hash_table->table[position].data[in_list_position];
}

static ssize_t find_elem_in_list_custom(const ListData* list, const elem_t element)
{
    assert(list);
    assert(element);

    ssize_t address = list->next[list->head];

    __m256i* element_data = (__m256i*) element;

    for (size_t i = 0; i < list->len - 1; i++)
    {
        __m256i* str_data = (__m256i*) list->data[address];

        if (!strcmp_custom(*element_data, *str_data)) return address;

        address = list->next[address];
    }

    return -1;
}

/*
static int strcmp_custom(const __m256i* first, const __m256i* second)
{
    return ~_mm256_movemask_epi8(_mm256_cmpeq_epi8(*first, *second));
}
*/