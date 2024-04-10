#include <stdio.h>
#include "hash_table.h"

int main()
{
    HashTable hash_table = {};
    hash_table_ctor(&hash_table, 2001);

    HashTableErrorCode error = NO_HASH_TABLE_ERROR;

    //prepare_text_file("src_text.txt", "dst_text.txt");

    textData text = {};

    read_file_to_text(&text, "dst_text.txt");

    load_hash_table_from_text(&hash_table, &text, crc32_hash);

    make_load_research(&hash_table, "load_res.csv");

    if ((error = make_search_test(&hash_table, &text, crc32_hash)))
    {
        print_hash_table_error(stderr, error);
    }

    //printf("Find: %s\n", find_in_hash_table(&hash_table, text.linesPtr[2], control_sum_hash));

    remove_text(&text);
    hash_table_dtor(&hash_table);

    return 0;
}