#include "huffman.h"
#include "huffmantree.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

	/*output tree to file->output :
	* number of chars char freq char freq...
	* char is 1 byte, freq is 
	*/
	fprintf(fp, "%d ", heap->index);

	for (i = 0; i < heap->index; i++) {
		fputc(heap->characters[i], fp);
		fprintf(fp, "%lld ", heap->frequencies[i]);
	}

	/*output codes to file*/
	i = 0;
	counter = 0;
	while (string[i] != '\0') {
		int j = 0;
		
		while (get_code(string[i])[j] != '\0') {
			if (counter == 8) {
				write_byte_to_file(fp, buffer);
				buffer = NULL;
				counter = 0;
			}
			if (get_code(string[i])[j] == '1') {
				buffer = set_bit(buffer, counter);
			}
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

void decode(char *input, char *output) {
	int i;

	FILE *fp = fopen(input, "r");
	char temp_char;
	long long temp_freq;
	int index;

	binary_heap *heap;
	node *n;

	if (!fp) {
		printf("error opening file");
	}

	/*read tree, rebuild heap and build tree*/
	heap = create_binary_heap(10);

	fscanf(fp, "%d ", &index);

	for (i = 0; i < index; i++) {
		temp_char = fgetc(fp);
		fscanf(fp, "%lld ", &temp_freq);
		add_node(heap, create_node(temp_char, temp_freq));
	}
	n = build_tree(heap);
	build_char_code(n);

	/*read codes and reconstruct text*/

	fclose(fp);
	destroy_binary_heap(heap);
}