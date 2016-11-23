#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
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

void question4() {
	/*question 2.4: compression rate*/
	FILE *fp;
	fp = fopen("small_jumps_small.txt", "w");
	generate_random_file(fp, 100000, 1);
	fclose(fp);
	fp = fopen("medium_jumps_small.txt", "w");
	generate_random_file(fp, 100000, 2);
	fclose(fp);
	fp = fopen("big_jumps_small.txt", "w");
	generate_random_file(fp, 100000, 3);
	fclose(fp);
	fp = fopen("small_jumps_medium.txt", "w");
	generate_random_file(fp, 500000, 1);
	fclose(fp);
	fp = fopen("medium_jumps_medium.txt", "w");
	generate_random_file(fp, 500000, 2);
	fclose(fp);
	fp = fopen("big_jumps_medium.txt", "w");
	generate_random_file(fp, 500000, 3);
	fclose(fp);
	fp = fopen("small_jumps_big.txt", "w");
	generate_random_file(fp, 1000000, 1);
	fclose(fp);
	fp = fopen("medium_jumps_big.txt", "w");
	generate_random_file(fp, 1000000, 2);
	fclose(fp);
	fp = fopen("big_jumps_big.txt", "w");
	generate_random_file(fp, 1000000, 3);
	fclose(fp);

	start_timer();
	encode("small_jumps_small.txt", "small_jumps_small_encoded");
	decode("small_jumps_small_encoded", "small_jumps_small_reconstructed");
	printf("small_jumps_small encoded and decoded \n");
	print_time();
	start_timer();
	encode("medium_jumps_small.txt", "medium_jumps_small_encoded");
	decode("medium_jumps_small_encoded", "medium_jumps_small_reconstructed");
	printf("medium_jumps_small encoded and decoded \n");
	print_time();
	start_timer();
	encode("big_jumps_small.txt", "big_jumps_small_encoded");
	decode("big_jumps_small_encoded", "big_jumps_small_reconstructed");
	printf("big_jumps_small encoded and decoded \n");
	print_time();
	start_timer();
	encode("small_jumps_medium.txt", "small_jumps_medium_encoded");
	decode("small_jumps_medium_encoded", "small_jumps_medium_reconstructed");
	printf("small_jumps_medium encoded and decoded \n");
	print_time();
	start_timer();
	encode("medium_jumps_medium.txt", "medium_jumps_medium_encoded");
	decode("medium_jumps_medium_encoded", "medium_jumps_medium_reconstructed");
	printf("medium_jumps_medium encoded and decoded \n");
	print_time();
	start_timer();
	encode("big_jumps_medium.txt", "big_jumps_medium_encoded");
	decode("big_jumps_medium_encoded", "big_jumps_medium_reconstructed");
	printf("big_jumps_medium encoded and decoded \n");
	print_time();
	start_timer();
	encode("small_jumps_big.txt", "small_jumps_big_encoded");
	decode("small_jumps_big_encoded", "small_jumps_big_reconstructed");
	printf("small_jumps_big encoded and decoded \n");
	print_time();
	start_timer();
	encode("medium_jumps_big.txt", "medium_jumps_big_encoded");
	decode("medium_jumps_big_encoded", "medium_jumps_big_reconstructed");
	printf("medium_jumps_big encoded and decoded \n");
	print_time();
	start_timer();
	encode("big_jumps_big.txt", "big_jumps_big_encoded");
	decode("big_jumps_big_encoded", "big_jumps_big_reconstructed");
	printf("big_jumps_big encoded and decoded \n");
	print_time();

	printf("checking files... \n");
	check_equals("small_jumps_small.txt", "small_jumps_small_reconstructed");
	check_equals("medium_jumps_small.txt", "medium_jumps_small_reconstructed");
	check_equals("big_jumps_small.txt", "big_jumps_small_reconstructed");
	check_equals("small_jumps_medium.txt", "small_jumps_medium_reconstructed");
	check_equals("medium_jumps_medium.txt", "medium_jumps_medium_reconstructed");
	check_equals("big_jumps_medium.txt", "big_jumps_medium_reconstructed");
	check_equals("small_jumps_big.txt", "small_jumps_big_reconstructed");
	check_equals("medium_jumps_big.txt", "medium_jumps_big_reconstructed");
	check_equals("big_jumps_big.txt", "big_jumps_big_reconstructed");
	printf("all files correctly encoded and decoded \n");
}

void run_tests() {
	/*add all tests here*/

	FILE *fp = fopen("data.txt", "w");
	fprintf(fp, "[0,12,34,567,890]");
	fclose(fp);

	printf("running tests... \n");

	encode_decode_test();

	printf("tests completed \n");

	question4();
}