#ifndef COMMON_H
#define COMMON_H

/*returns input sequence with ',' as delimeter
* don't read all the bytes at once, read from 'from' to 'to'
*/
char* read_JSON_from_file(const char* filename, long from, long number_of_bytes);
/*returns 0 if succeeded, 1 otherwise*/
int write_to_file(const char* content, const char* filename);

#endif