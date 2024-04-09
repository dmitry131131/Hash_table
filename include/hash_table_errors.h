#ifndef HASH_TABLE_ERRORS_H
#define HASH_TABLE_ERRORS_H

enum HashTableErrorCode {
    NO_HASH_TABLE_ERROR,
    WRONG_CAPACITY,
    ALLOC_MEMORY_ERROR,
    LIST_DTOR_ERROR,
    LIST_CTOR_ERROR,
    INSERT_IN_LIST_ERROR,
    CREATE_FILE_ERROR,
    WRITE_TO_FILE_ERROR
};

void print_hash_table_error(FILE* stream, HashTableErrorCode error);
void print_hash_table_error_message(FILE* stream, HashTableErrorCode error);

#endif