#ifndef HASH_FUNCTIONS_H
#define HASH_FUNCTIONS_H

size_t zero_hash(const char* str, size_t table_len);
size_t first_alpha_hash(const char* str, size_t table_len);
size_t len_hash(const char* str, size_t table_len);
size_t control_sum_hash(const char* str, size_t table_len);
size_t ror_hash(const char* str, size_t table_len);
size_t crc32_hash(const char* str, size_t table_len); 

size_t crc32_hash_simd(const char* str, size_t table_len);
size_t crc32a(const char* str, size_t table_len);

#endif