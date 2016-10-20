#include "common.h"
#include <stdio.h>
#include <stdlib.h>

char* read_JSON_from_file(const char* filename, long from, long number_of_bytes) {
	char* result;
	char temp;
	int valid_chars = 0;
	int i = 0;
	
	FILE *fp = fopen(filename, "r");

	if (!fp) {
		printf("Error opening file: %s", filename);
	}
	
	/*start reading from correct position*/
	fseek(fp, from, SEEK_SET);

	/*allocate memory for all the characters*/
	result = (char*) malloc(sizeof(char)*(number_of_bytes + 1));

	if (!result) {
		printf("Error while allocating memory");
	}

	temp = fgetc(fp);
	while (temp != EOF && i < number_of_bytes) {
		if (temp != '[' && temp != ']') { /*json chars that don't need to be stored*/
			result[i] = temp;
			i++;
		}
		temp = fgetc(fp);
	}
	
	fclose(fp);
	result[i] = '\0';
	return result;
}

int write_string_to_file(const char* content, const char* filename) {
	FILE *fp = fopen(filename, "w");
	int result;

	if (!fp) {
		printf("Error opening file: %s", filename);
	}

	result = fputs(content, fp);

	fclose(fp);

	if (result < 0) {
		return 1; /*an error occured*/
	}
	else {
		return 0;
	}
}

void write_byte_to_file(FILE *fp, char byte) {
	if (!fp) {
		printf("error: file must be opened");
	}

	fputc(byte, fp);
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
