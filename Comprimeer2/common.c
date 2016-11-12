#include "common.h"
#include <stdio.h>
#include <stdlib.h>

char* read_bytes_from_file(FILE* fp, long from, long number_of_bytes) {
	char* result;
	char temp;
	int i = 0;
	
	/*start reading from correct position*/
	fseek(fp, from, SEEK_SET);

	/*allocate memory for all the characters*/
	result = (char*) malloc(sizeof(char)*(number_of_bytes + 1));

	if (!result) {
		printf("Error while allocating memory");
	}

	temp = fgetc(fp);
	while (temp != EOF && i < number_of_bytes) {
		result[i] = temp;
		i++;
		temp = fgetc(fp);
	}
	
	result[i] = '\0';
	return result;
}

char set_bit(char byte, int pos) {
	byte |= 1 << pos;
	return byte;
}

char clear_bit(char byte, int pos) {
	byte &= ~(1 << pos);
	return byte;
}

int test_bit(const char byte, int pos) {
	return ((byte & (1 << pos)) != 0);
}
