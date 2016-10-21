#include "huffman.h"
#include "huffmantree.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h>

void encode(char *input, char *output) {
	int i = 0;
	int counter = 0;
	char buffer = NULL;

	char *string;
	binary_heap* heap;
	node *n;

	string = read_JSON_from_file(input, 0, 1000);
	heap = create_binary_heap(10);
	count_frequencies(heap, string);
	add_to_heap(heap);
	n = build_tree(heap);
	build_char_code(n);

	FILE *fp = fopen(output, "w");

	while (string[i] != '\0') {
		int j = 0;

		printf("char: %c -> code: %s\n", string[i], get_code(string[i]));
		
		while (get_code(string[i])[j] != '\0') {
			if (counter == 8) {
				write_byte_to_file(fp, buffer);
				buffer = NULL;
				counter = 0;
			}
			buffer = set_bit(buffer, counter);
			counter++;

			j++;
		}
		i++;
	}
	
	buffer = EOF;
	fputc(buffer, fp);
	fclose(fp);

	destroy_binary_heap(heap);
	free(string);
}