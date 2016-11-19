#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#include "timer.h"
#include "encode_decode.h"

/*jump_size is 1, 2 or 3, with 1 smallest jumps to 3 biggest jumps*/
void generate_random_file(FILE *fp, long long size, int jump_size) {
	long long i;
	long long delta = 0;
	long long tmp = rand();
	fprintf(fp, "[");
	for (i = 0; i < size && tmp > 0; i++) { /*temp is smaller then 0 when it becomes larger than max long long*/
		fprintf(fp, "%lld,", tmp);
		delta = pow(rand(), jump_size);
		tmp += delta;
	}
	if (tmp > 0) {
		fprintf(fp, "%lld]%c", tmp, EOF);
	}
	else {
		fprintf(fp, "%lld]%c", LLONG_MAX, EOF);
	}
}

void encode_decode_test() {
	char char1;
	char char2;

	encode("data.txt", "compressed"); /*contains [0,12,34,567,890]*/
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

/*check if two files are equal*/
void check_equals(char* input, char* output) {
	FILE *fp1 = fopen(input, "r");
	FILE *fp2 = fopen(output, "r");
	char char1;
	char char2;

	while ((char1 = fgetc(fp1)) != EOF) { /*check if all characters match*/
		char2 = fgetc(fp2);
		assert(char1 == char2);
	}
}

void run_tests() {
	/*add all tests here*/

	FILE *fp = fopen("data.txt", "w");
	fprintf(fp, "[0,12,34,567,890]");
	fclose(fp);

	printf("running tests... \n");

	encode_decode_test();

	printf("tests completed \n");

	/*question 3: compression rate*/
	fp = fopen("small_jumps.txt", "w");
	generate_random_file(fp, 1000000, 1);
	fclose(fp);
	fp = fopen("medium_jumps.txt", "w");
	generate_random_file(fp, 1000000, 2);
	fclose(fp);
	fp = fopen("big_jumps.txt", "w");
	generate_random_file(fp, 1000000, 3);
	fclose(fp);

	start_timer();
	encode("small_jumps.txt", "small_jumps_encoded");
	decode("small_jumps_encoded", "small_jumps_reconstructed");
	printf("small_jumps encoded and decoded \n");
	print_time();
	start_timer();
	encode("medium_jumps.txt", "medium_jumps_encoded");
	decode("medium_jumps_encoded", "medium_jumps_reconstructed");
	printf("medium_jumps encoded and decoded \n");
	print_time();
	start_timer();
	encode("big_jumps.txt", "big_jumps_encoded");
	decode("big_jumps_encoded", "big_jumps_reconstructed");
	printf("big_jumps encoded and decoded \n");
	print_time();

	printf("checking files... \n");
	check_equals("small_jumps.txt", "small_jumps_reconstructed");
	check_equals("medium_jumps.txt", "medium_jumps_reconstructed");
	check_equals("big_jumps.txt", "big_jumps_reconstructed");
	printf("all files correctly encoded and decoded \n");
}