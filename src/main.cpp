#include <stdio.h>
#include "hash_table.h"

int main()
{
    HashTable hash_table = {};
    hash_table_ctor(&hash_table, 5001);

    char text[8] = "hui\n";
    char text1[8] = "gay\n";

    add_to_hash_table(&hash_table, text, control_sum_hash);

    printf("Find: %s\n", find_in_hash_table(&hash_table, text1, control_sum_hash));

    prepare_text_file("src_text.txt", "dst_text.txt");

    outputBuffer src_buffer = {};

    FILE* in_file = fopen("dst_text.txt", "r");
    read_file_in_buffer(&src_buffer, in_file);
    fclose(in_file);

    load_hash_table_from_buffer(&hash_table, &src_buffer, control_sum_hash);

    make_load_research(&hash_table, "load_res.csv");

    buffer_dtor(&src_buffer);
    hash_table_dtor(&hash_table);

    return 0;
}