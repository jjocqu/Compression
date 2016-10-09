#ifndef COMMON_H
#define COMMON_H

/*returns input sequence with whitespace as delimeter*/
char* read_JSON_from_file(const char* filename);
/*returns 0 if succeeded, 1 otherwise*/
int write_to_file(const char* content, const char* filename);

#endif