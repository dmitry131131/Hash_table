#ifndef HASH_TEST_H
#define HASH_TEST_H

const size_t word_size = 32;

struct HashText {
    size_t bufferSize;  ///< Size of text buffer
    size_t linesCount;  ///< Count of lines in text
    char*  bufferName;  ///< Name of buffer
    char** linesPtr;    ///< Name of lines array
};

HashTableErrorCode make_load_research(HashTable* hash_table, const char* filename);

HashTableErrorCode prepare_text_file(const char* src_filename, const char* dst_filename);

HashTableErrorCode load_hash_table_from_text(HashTable* hash_table, HashText* text, size_t (*hash_function)(const char*, size_t));

HashTableErrorCode read_file_to_text(HashText* text, const char* filename);
HashTableErrorCode hash_text_dtor(HashText* text);

HashTableErrorCode make_search_test(HashTable* hash_table, HashText* text, size_t (*hash_function)(const char*, size_t));

#endif