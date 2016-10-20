#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

/*returns input sequence with ',' as delimeter
* don't read all the bytes at once, read from 'from' to 'to'
*/
char* read_JSON_from_file(const char* filename, long from, long number_of_bytes);
/*returns 0 if succeeded, 1 otherwise*/
int write_string_to_file(const char* content, const char* filename);
/*write byte per byte to file
* file must be opened and closed outside this function
*/
void write_byte_to_file(FILE *fp, char byte);

/*bit operations*/
char set_bit(char byte, int pos);
char clear_bit(char byte, int pos);
int test_bit(const char byte, int pos);

#endif