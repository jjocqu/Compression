#include "huffman.h"
#include "huffmantree.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h>

void encode(char *input, char *output) {
	int i = 0;

	char *string;
	binary_heap* heap;
	node *n;

	string = read_JSON_from_file(input, 0, 1000);
	heap = create_binary_heap(10);
	count_frequencies(heap, string);
	add_to_heap(heap);
	n = build_tree(heap);
	build_char_code(n);

	while (string[i] != '\0') {
		
		printf("char: %c -> code: %s\n", string[i], get_code(string[i]));
		
		i++;
	}
	

	destroy_binary_heap(heap);
	free(string);
}