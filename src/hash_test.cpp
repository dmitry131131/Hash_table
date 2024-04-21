#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include "DataBuffer.h"
#include "hash_table.h"

HashTableErrorCode make_load_research(HashTable* hash_table, const char* filename)
{
    assert(hash_table);

    FILE* output_file = nullptr;

    if (create_output_file(&output_file, filename, TEXT)) return CREATE_FILE_ERROR;

    outputBuffer buffer = {.filePointer = output_file, .AUTO_FLUSH = true};

    for (size_t i = 0; i < hash_table->capacity; i++)
    {
        if (hash_table->table[i].capacity)
            print_to_buffer(&buffer, "%lu\n", hash_table->table[i].len - 1);
        else
            print_to_buffer(&buffer, "0\n");
    }

    if (write_buffer_to_file(&buffer)) return WRITE_TO_FILE_ERROR;

    return NO_HASH_TABLE_ERROR;
}

HashTableErrorCode prepare_text_file(const char* src_filename, const char* dst_filename)
{
    assert(src_filename);
    assert(dst_filename);

    FILE* src_file = fopen(src_filename, "r");
    FILE* dst_file = fopen(dst_filename, "w");

    struct stat buff = {};

    fstat(fileno(src_file), &buff);

    if (buff.st_size < 0) {}

    size_t text_size = (size_t) buff.st_size;

    bool is_new_line = false;
    for (size_t i = 0; i < text_size; i++)
    {
        char symbol = (char) getc(src_file);
        if (isalpha(symbol))  
        {                 
            if (symbol <= 'Z')
                putc(symbol + ('a' - 'A'), dst_file);
            else
                putc(symbol, dst_file);
            is_new_line = false;
        }
        if ((symbol == ' ' || symbol == '\n') && !is_new_line)  
        {
            putc('\n', dst_file);
            is_new_line = true;
        }
    }

    fclose(src_file);
    fclose(dst_file);

    return NO_HASH_TABLE_ERROR;
}

HashTableErrorCode load_hash_table_from_text(HashTable* hash_table, HashText* text, size_t (*hash_function)(const char*, size_t))
{
    assert(hash_table);
    assert(text);

    for (size_t i = 0; i < text->linesCount; i++)
    {
        add_to_hash_table(hash_table, text->linesPtr[i], hash_function);
    } 

    return NO_HASH_TABLE_ERROR;
}

HashTableErrorCode read_file_to_text(HashText* text, const char* filename)
{
    assert(text);
    assert(filename);

    textData src_text = {};

    if (get_text(filename, &src_text)) return TEXT_READ_ERROR;

    text->bufferSize = src_text.linesCount * word_size;
    text->linesCount = src_text.linesCount;
    text->bufferName = (char*)  aligned_alloc(word_size, text->bufferSize * sizeof(char));
    text->bufferName = (char*)  memset(text->bufferName, 0, text->bufferSize);
    text->linesPtr   = (char**) calloc(src_text.linesCount, sizeof(char*));
    if (!text->bufferName) return ALLOC_MEMORY_ERROR;
    if (!text->linesPtr)   return ALLOC_MEMORY_ERROR;

    for (size_t i = 0; i < src_text.linesCount; i++)
    {   
        text->linesPtr[i] = text->bufferName + i * word_size;
        strncpy(text->linesPtr[i], src_text.linesPtr[i], word_size);
    }

    remove_text(&src_text);

    return NO_HASH_TABLE_ERROR;
}

HashTableErrorCode hash_text_dtor(HashText* text)
{
    assert(text);

    text->bufferSize = 0;
    text->linesCount = 0;
    free(text->bufferName);
    free(text->linesPtr);

    return NO_HASH_TABLE_ERROR;
}

HashTableErrorCode make_search_test(HashTable* hash_table, HashText* text, size_t (*hash_function)(const char*, size_t))
{
    assert(hash_table);
    assert(text);

    for (size_t _ = 0; _ < 500; _++)
    {    
        for (size_t i = 0; i < 100000; i++)
        {
            if (!find_in_hash_table(hash_table, text->linesPtr[i], hash_function)) return HASH_TABLE_TEST_ERROR;
        }
    }

    return NO_HASH_TABLE_ERROR;
}