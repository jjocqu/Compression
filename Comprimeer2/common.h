#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

/*returns input sequence with ',' as delimeter
* don't read all the bytes at once
*/
char* read_bytes_from_file(FILE* fp, long from, long number_of_bytes);

/*bit operations*/
char set_bit(char byte, int pos);
char clear_bit(char byte, int pos);
int test_bit(const char byte, int pos);

#endif