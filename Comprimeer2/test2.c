#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "encode_decode.h"

void encode_decode_test() {
	char char1;
	char char2;

	encode("data.txt", "compressed"); /*contains 0,12,34,567,890*/
	decode("compressed", "reconstructed.txt");

	FILE *fp1 = fopen("data.txt", "r");
	FILE *fp2 = fopen("reconstructed.txt", "r");

	while ((char1 = fgetc(fp1)) != EOF) { /*check if all characters match*/
		char2 = fgetc(fp2);
		assert(char1 == char2);
	}

	fclose(fp1);
	fclose(fp2);
}

void run_tests() {
	/*add all tests here*/

	FILE *fp = fopen("data.txt", "w");
	fprintf(fp, "0,12,34,567,890");
	fclose(fp);

	printf("running tests... \n");

	encode_decode_test();

	printf("tests completed \n");
}