#ifndef HASH_TEST_H
#define HASH_TEST_H

HashTableErrorCode make_load_research(HashTable* hash_table, const char* filename);

HashTableErrorCode prepare_text_file(const char* src_filename, const char* dst_filename);

HashTableErrorCode load_hash_table_from_buffer(HashTable* hash_table, outputBuffer* buffer, size_t (*hash_function)(const char*, size_t));

#endif