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

	int garbage_zeros = 0;

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
	/*output last byte (with added zero's) and output number of added zero's*/
	write_byte_to_file(fp, buffer);
	garbage_zeros = 8 - counter;
	write_byte_to_file(fp, garbage_zeros + '0');
	write_byte_to_file(fp, EOF);
	fclose(fp);

	destroy_binary_heap(heap);
	free(string);
}

void decode(char *input, char *output) {
	int i;

	FILE *fpi = fopen(input, "r");
	long long input_length;
	FILE *fpo = fopen(output, "w");
	char temp_char;
	long long temp_freq;
	int index;
	char buffer;
	char code[256];
	int code_index = 0;

	int garbage_zeros;
	char last;
	int garbage_chars = 0;

	binary_heap *heap;
	node *n;

	if (!fpi) {
		printf("error opening file");
	}

	/*first read number of garbage zero's at the end*/
	fseek(fpi, 0, SEEK_END);
	input_length = ftell(fpi);
	fseek(fpi, (input_length - 2), SEEK_SET);
	last = fgetc(fpi);
	garbage_zeros = last - '0';

	rewind(fpi);

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

	while (buffer != EOF && buffer != last) {
		int j = 0;
		char result;
		int offset = 0;

		if (ftell(fpi) == (input_length - 2)) { /*second last byte: byte with garbage zeros!*/
			offset = garbage_zeros; /*last 'offset' bits are garbage zeros*/
		}
		for (j = 0; j < 8 - offset; j++) { /*check all bits in buffer*/
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
				printf("%c", result);
				code_index = 0;
			}
		}
		buffer = fgetc(fpi);
	}

	/*delete unwanted characters added by garbage zeros*/
	/*code_index = 0;
	for (i = 0; i < garbage_zeros; i++) {
		code[code_index] = '0';
		code_index++;
		code[code_index] = '\0';
		if (get_char(n, code)) {
			garbage_chars++;
			code_index = 0;
		}
	}
	fseek(fpo, -2, SEEK_CUR);
	write_byte_to_file(fpo, ']');
	write_byte_to_file(fpo, '\0');
	write_byte_to_file(fpo, '\0');*/

	write_byte_to_file(fpo, ']');

	fclose(fpi);
	fclose(fpo);
	destroy_binary_heap(heap);
}