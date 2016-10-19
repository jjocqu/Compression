#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#include <string.h>
#include "common.h"
#include "timer.h"
#include "test.h"

int main(int argc, char **argv) {

	if (strcmp(argv[1], "-t") == 0) { /*run tests*/
		run_tests();
		_CrtDumpMemoryLeaks();
		return 0;
	}

	if (argc < 3) {
		printf("Error 3 arguments necessary: -c/-d inputfile outputfile \n");
		return 1;
	}

	if (strcmp(argv[1], "-c") == 0) {

		printf("comprimeer %s tot %s \n", argv[2], argv[3]);

		start_timer();

		/*read and write test*/
		char *string = read_JSON_from_file(argv[2], 0, 1000);
		write_to_file(string, argv[3]);

		printf("reading and writing to file time: \n");
		print_time();

	}
	else if (strcmp(argv[1], "-d") == 0) {
		printf("decodeer %s tot %s \n", argv[2], argv[3]);
	}

	return 0;
}