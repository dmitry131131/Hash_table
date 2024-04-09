#include <stdio.h>
#include <assert.h>
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

HashTableErrorCode load_hash_table_from_buffer(HashTable* hash_table, outputBuffer* buffer, size_t (*hash_function)(const char*, size_t))
{
    assert(hash_table);
    assert(buffer);

    size_t str_start_pointer = 0;

    for (size_t i = 0; i < buffer->customSize; i++)
    {
        if (buffer->customBuffer[i] == '\n')
        {
            buffer->customBuffer[i] = 0;

            add_to_hash_table(hash_table, &(buffer->customBuffer[str_start_pointer]), hash_function);

            str_start_pointer = i + 1;
        } 
    }

    return NO_HASH_TABLE_ERROR;
}