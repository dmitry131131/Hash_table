#ifndef HASH_TABLE_H
#define HASH_TABLE_H

const size_t DEFAULT_LIST_SIZE = 10;

#include "List.h"
#include "DataBuffer.h"
#include "OneginError.h"
#include "InputOutput.h"
#include "hash_table_errors.h"

struct HashTable {
    ListData* table;
    size_t    capacity;
};

HashTableErrorCode hash_table_ctor(HashTable* hash_table, size_t capacity);
HashTableErrorCode hash_table_dtor(HashTable* hash_table);

HashTableErrorCode add_to_hash_table(HashTable* hash_table, char* str, size_t (*hash_function)(const char*, size_t));
char* find_in_hash_table(HashTable* hash_table, char* str, size_t (*hash_function)(const char*, size_t));
char* find_in_hash_table_inline(HashTable* hash_table, char* str);

#include "hash_functions.h"
#include "hash_test.h"

#endif