#include <stdio.h>

#include "hash_table_errors.h"
#include "Color_output.h"

void print_hash_table_error(FILE* stream, HashTableErrorCode error)
{
    print_hash_table_error_message(stream, error);
}

void print_hash_table_error_message(FILE* stream, HashTableErrorCode error)
{
    #define CHECK_CODE(code, message)                                               \
        case code:                                                                  \
            color_fprintf(stream, COLOR_RED, STYLE_BOLD, "Hash table error: ");     \
            fprintf(stream, message);                                               \
            break;                                                                  \

    switch (error)
    {
    case NO_HASH_TABLE_ERROR:
        break;

    CHECK_CODE(WRONG_CAPACITY,          "Wrong capacity given!\n");
    CHECK_CODE(ALLOC_MEMORY_ERROR,      "Alloc memory error(malloc returned NULL)!\n");
    CHECK_CODE(LIST_DTOR_ERROR,         "Can't destroy one element in hash table!\n");
    CHECK_CODE(LIST_CTOR_ERROR,         "Can't create one element in hash table!\n");
    CHECK_CODE(INSERT_IN_LIST_ERROR,    "Can't insert element in list!\n");
    CHECK_CODE(CREATE_FILE_ERROR,       "Create output file error!\n");
    CHECK_CODE(WRITE_TO_FILE_ERROR,     "Write buffer to file error!\n");
    CHECK_CODE(TEXT_READ_ERROR,         "Error in reading text process!\n");
    CHECK_CODE(HASH_TABLE_TEST_ERROR,   "Error in testing hash table!\n");
    
    
    default:
        fprintf(stream, "Unknown error!\n");
        break;
    }

    #undef CHECK_CODE
}