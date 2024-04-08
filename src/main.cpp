#include <stdio.h>
#include "hash_table.h"

int main()
{
    HashTable hash_table = {};
    hash_table_ctor(&hash_table, 5);

    char text[8] = "hui\n";
    char text1[8] = "gay\n";

    add_to_hash_table(&hash_table, text, control_sum_hash);

    printf("Find: %s\n", find_in_hash_table(&hash_table, text1, control_sum_hash));

    hash_table_dtor(&hash_table);

    return 0;
}