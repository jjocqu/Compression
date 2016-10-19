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

	/*start reading from position*/
	fseek(fp, from, SEEK_SET);

	/*count the number of character in the json file*/
	temp = fgetc(fp);
	while (temp != EOF && valid_chars < number_of_bytes) {
		if (temp != '[' && temp != ']') { /*json chars that don't need to be stored*/
			valid_chars++;
		}
		temp = fgetc(fp);
	}

	/*allocate memory for all the characters*/
	result = (char*) malloc(sizeof(char)*(valid_chars + 1));

	if (!result) {
		printf("Error while allocating memory");
	}

	rewind(fp); /*resets file pointer to beginning*/

	temp = fgetc(fp);
	while (temp != EOF && i < number_of_bytes) {
		if (temp != '[' && temp != ']') { /*json chars that don't need to be stored*/
			result[i] = temp;
			i++;
		}
		temp = fgetc(fp);
	}
	
	fclose(fp);
	result[valid_chars] = '\0';
	return result;
}

int write_to_file(const char* content, const char* filename) {
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