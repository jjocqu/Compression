#include <stdio.h>
#include <string.h>
#include "timer.h"
#include "test.h"
#include "encode_decode.h"

int main(int argc, char **argv) {

	if (strcmp(argv[1], "-t") == 0) { /*run tests*/
		run_tests();
		return 0;
	}

	if (argc < 3) {
		printf("Error 3 arguments necessary: -c/-d inputfile outputfile \n");
		return 1;
	}

	if (strcmp(argv[1], "-c") == 0) {

		printf("comprimeer %s tot %s \n", argv[2], argv[3]);

		start_timer();
		encode(argv[2], argv[3]);
		print_time();

	}
	else if (strcmp(argv[1], "-d") == 0) {
		printf("decodeer %s tot %s \n", argv[2], argv[3]);
		start_timer();
		decode(argv[2], argv[3]);
		print_time();
	}

	return 0;
}