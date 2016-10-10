#include "common.h"
#include <stdio.h>
#include <stdlib.h>

long long number_of_chars;

char* read_JSON_from_file(const char* filename) {
	char* result;
	char temp;
	int valid_chars = 0;
	int i = 0;
	
	number_of_chars = 0;

	FILE *fp = fopen(filename, "r");

	if (!fp) {
		printf("Error opening file: %s", filename);
	}

	/*count the number of character in the json file*/
	temp = fgetc(fp);
	number_of_chars++;
	while (temp != EOF) {
		if (temp != ',' && temp != '[' && temp != ']') { /*json chars that don't need to be stored*/
			valid_chars++;
		}
		temp = fgetc(fp);
		number_of_chars++;
	}
	number_of_chars--; /*????TODO telt EOF mee als char???*/

	/*allocate memory for all the characters*/
	result = malloc(sizeof(char)*(valid_chars + 1));
	rewind(fp); /*resets file pointer to beginning*/

	temp = fgetc(fp);
	while (temp != EOF) {
		if (temp != ',' && temp != '[' && temp != ']') { /*json chars that don't need to be stored*/
			result[i] = temp;
			i++;
		}
		temp = fgetc(fp);
	}

	fclose(fp);

	result[valid_chars] = '\0';
	return result;
}

long long get_number_of_chars() {
	return number_of_chars;
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