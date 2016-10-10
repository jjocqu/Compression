#ifndef COMMON_H
#define COMMON_H

/*returns input sequence with whitespace as delimeter*/
char* read_JSON_from_file(const char* filename);
/*returns total number of chars of the last read file
* do not use before read_JSON_from_file
*/
long long get_number_of_chars();
/*returns 0 if succeeded, 1 otherwise*/
int write_to_file(const char* content, const char* filename);

#endif