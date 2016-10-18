#include "huffman.h"
#include "huffmantree.h"

#include <stdio.h>

void encode(char *input) {
	int i = 0;

	char *string;
	binary_heap* heap;
	node *n;

	string = read_JSON_from_file(input);
	heap = build_heap(string);
	n = build_tree(heap);
	build_char_code(n);

	while (string[i] != '\0') {
		
		printf("char: %c -> code: %s\n", string[i], get_code(string[i]));
		
		i++;
	}
	

	destroy_binary_heap(heap);
	free(string);
}