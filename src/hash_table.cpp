#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "hash_table.h"

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

    if (list_insert_in_tail(&(hash_table->table[position]), str)) return INSERT_IN_LIST_ERROR;

    return NO_HASH_TABLE_ERROR;
}

char* find_in_hash_table(HashTable* hash_table, char* str, size_t (*hash_function)(const char*, size_t))
{
    assert(hash_table);
    assert(str);

    size_t position = hash_function(str, hash_table->capacity);

    if (!(hash_table->table[position].capacity)) return nullptr;

    ssize_t in_list_position = find_elem_in_list(&(hash_table->table[position]), str);

    if (in_list_position == -1) return nullptr;

    return hash_table->table[position].data[in_list_position];
}