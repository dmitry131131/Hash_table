#ifndef HASH_TEST_H
#define HASH_TEST_H

HashTableErrorCode make_load_research(HashTable* hash_table, const char* filename);

HashTableErrorCode prepare_text_file(const char* src_filename, const char* dst_filename);

HashTableErrorCode load_hash_table_from_text(HashTable* hash_table, textData* text, size_t (*hash_function)(const char*, size_t));

HashTableErrorCode read_file_to_text(textData* text, const char* filename);

HashTableErrorCode make_search_test(HashTable* hash_table, textData* text, size_t (*hash_function)(const char*, size_t));

#endif