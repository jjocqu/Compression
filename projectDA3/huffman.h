#ifndef HUFFMAN_H
#define HUFFMAN_H

/*we store code as char[] but of course that is not how we will write it to the file*/
typedef struct char_code {
	char character;
	char *code;
	int code_lenght;
} char_code;

#endif