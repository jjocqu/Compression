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
	/*output last byte (with added zero's)*/
	write_byte_to_file(fp, buffer);
	buffer = NULL;
	counter = 0;

	buffer = EOF;
	fputc(buffer, fp);
	fclose(fp);

	destroy_binary_heap(heap);
	free(string);
}

void decode(char *input, char *output) {
	int i;

	FILE *fpi = fopen(input, "r");
	FILE *fpo = fopen(output, "w");
	char temp_char;
	long long temp_freq;
	int index;
	char buffer = NULL;
	char code[256];
	int code_index = 0;

	binary_heap *heap;
	node *n;

	if (!fpi) {
		printf("error opening file");
	}

	/*read tree, rebuild heap and build tree*/
	heap = create_binary_heap(10);

	fscanf(fpi, "%d ", &index);

	for (i = 0; i < index; i++) {
		temp_char = fgetc(fpi);
		fscanf(fpi, "%lld ", &temp_freq);
		add_node(heap, create_node(temp_char, temp_freq));
	}
	n = build_tree(heap);
	build_char_code(n);

	/*read codes and reconstruct text*/
	write_byte_to_file(fpo, '[');
	buffer = fgetc(fpi);

	while (buffer != EOF) {
		int j = 0;
		char result;

		for (j = 0; j < 8; j++) { /*check all bits in buffer*/
			if (test_bit(buffer, j)) {
				code[code_index] = '1';
			}
			else {
				code[code_index] = '0';
			}
			code_index++;
			code[code_index] = '\0'; 
			if (get_char(n, code)) {/*check if code is long enough*/
				result = get_char(n, code);
				write_byte_to_file(fpo, result);
				code_index = 0;
			}
		}

		buffer = fgetc(fpi);
	}

	write_byte_to_file(fpo, ']');

	fclose(fpi);
	fclose(fpo);
	destroy_binary_heap(heap);
}