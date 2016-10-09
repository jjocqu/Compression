#include <stdio.h>
#include <string.h>
#include "common.h"
#include <stdlib.h>
#include "timer.h"

int main(int argc, char **argv) {

	if (argc < 3) {
		printf("Error 3 arguments necessary: -c/-d inputfile outputfile \n");
		system("pause"); /*prevents console from closing*/
		return 1;
	}

	if (strcmp(argv[1], "-c") == 0) {
		printf("comprimeer %s tot %s \n", argv[2], argv[3]);

		start_timer();

		/*read and write test*/
		char *string = read_JSON_from_file(argv[2]);
		write_to_file(string, argv[3]);

		printf("reading and writing to file time: \n");
		print_time();
	}
	else if (strcmp(argv[1], "-d") == 0) {
		printf("decodeer %s tot %s \n", argv[2], argv[3]);
	}

	system("pause"); /*prevents console from closing*/
	return 0;
}