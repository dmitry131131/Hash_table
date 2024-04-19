#include <stdio.h>
#include "hash_table.h"
#include <immintrin.h>

int main()
{
    HashTable hash_table = {};
    hash_table_ctor(&hash_table, 2001);

    HashTableErrorCode error = NO_HASH_TABLE_ERROR;

    //prepare_text_file("src_text.txt", "dst_text.txt");

    HashText text = {};

    read_file_to_text(&text, "dst_text.txt");

    load_hash_table_from_text(&hash_table, &text, crc32_hash_simd);

    make_load_research(&hash_table, "load_res.csv");

    unsigned long start = __rdtsc();

    if ((error = make_search_test(&hash_table, &text, crc32_hash_simd)))
    {
        print_hash_table_error(stderr, error);
    }

    unsigned long end = __rdtsc();

    printf("TIME: %ld\n", end-start);

    hash_text_dtor(&text);
    hash_table_dtor(&hash_table);

    return 0;
}