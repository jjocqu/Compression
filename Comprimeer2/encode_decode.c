#include "encode_decode.h"
#include "huffmantree.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void print_char_code() {
	printf("printing code: \n");
	for (int i = 0; i < 256; i++) {
		if (char_code[i][0] != '\0') {
			printf("char: %c -> code: %s \n",i, char_code[i]);
		}
	}
	printf("\n");
}

void encode(char *input, char *output) {
	int i = 0;
	int counter = 0;
	char buffer = '\0';

	int garbage_zeros = 0;
	char temp;
	long long num;
	char string[21]; //long enough to store long long as string + ',' + '\0'

	binary_heap* heap;
	node *n;

	FILE* fpi;
	FILE* fpo;

	/*read [ and then keep reading a long long and a char (,) untill the char is ] */
	fpi = fopen(input, "rb");
	heap = create_binary_heap(12); /*11 chars: 0-9 and , */
	fgetc(fpi); /*read [ */
	fscanf(fpi, "%lld", &num);
	sprintf(string, "%lld", num);
	count_frequencies(heap, string);
	while ((temp = fgetc(fpi)) != ']') {
		fscanf(fpi, "%lld", &num);
		sprintf(string, ",%lld", num);
		count_frequencies(heap, string);
	}
	add_to_heap(heap);
	n = build_tree(heap);
	build_char_code(n);
	fpo = fopen(output, "wb");

	/*output tree to file->output :
	* numberofchars charfreqEOFcharfreqEOF...
	* char is 1 byte, freq is 
	*/
	fputc(heap->index, fpo);

	for (i = 0; i < heap->index; i++) {
		fputc(heap->characters[i], fpo);
		fprintf(fpo, "%lld %c", heap->frequencies[i], EOF); /*delimiter needs to be char that can't be in tree*/
	}

	/*output codes to file*/
	counter = 0;
	rewind(fpi);
	while ((temp = fgetc(fpi)) != ']') { /*loop over every number*/
		i = 0;
		fscanf(fpi, "%lld", &num);
		if (temp == '[') { /*first num -> no ','*/
			sprintf(string, "%lld", num);
		}
		else {
			sprintf(string, ",%lld", num);
		}
		while (string[i] != '\0') { /*loop over every char in string*/
			int j = 0;
			while (get_code(string[i])[j] != '\0') {
				if (counter == sizeof(char) * 8) {
					fputc(buffer, fpo);
					buffer = '\0';
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
	}
	/*output last byte (with added zero's) and output number of added zero's*/
	fputc(buffer, fpo);
	garbage_zeros = sizeof(char)*8 - counter;
	fputc(garbage_zeros + '0', fpo);
	fputc(EOF, fpo);
	fclose(fpi);
	fclose(fpo);

	destroy_binary_heap(heap);
}

void decode(char *input, char *output) {
	int i;

	FILE *fpi = fopen(input, "rb");
	long long input_length;
	FILE *fpo = fopen(output, "wb");
	char temp_char;
	long long temp_freq;
	int index;
	char buffer;
	unsigned char code[256];
	int code_index = 0;
	int last_byte_read = 0; /*boolean value*/

	int garbage_zeros;
	char last;
	int garbage_chars = 0;

	char string[21]; //long enough to store long long as string + ',' + '\0'
	int string_pos = 0;

	binary_heap *heap;
	node *n;

	if (!fpi || !fpo) {
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
	heap = create_binary_heap(12);

	/*read tree size*/
	index = fgetc(fpi);

	for (i = 0; i < index; i++) {
		char eof; /*read eof char*/
		temp_char = fgetc(fpi);
		fscanf(fpi, "%lld %c", &temp_freq, &eof);
		add_node(heap, create_node(temp_char, temp_freq));
	}
	n = build_tree(heap);
	build_char_code(n);
	/*read codes and reconstruct text*/
	fputc('[', fpo);
	buffer = fgetc(fpi);
	while (!last_byte_read) { 
		unsigned int j = 0;
		char result;
		int offset = 0;

		if (ftell(fpi) == (input_length - 2)) { /*second last byte: byte with garbage zeros!*/
			offset = garbage_zeros; /*last 'offset' bits are garbage zeros*/
			last_byte_read = 1;
		}
		for (j = 0; j < sizeof(char)*8 - offset; j++) { /*check all bits in buffer*/
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
				if (result == ',') { /*number is read*/
					string[string_pos] = '\0'; 
					fputs(string, fpo);
					fputc(result, fpo);
					string_pos = 0;
				}
				else {
					string[string_pos] = result;
					string_pos++;
				}
				
				code_index = 0;
			}
		}
		buffer = fgetc(fpi);
	}
	/*print last num*/
	string[string_pos] = '\0'; 
	fputs(string, fpo);
	fputc(']', fpo);

	fclose(fpi);
	fclose(fpo);
	destroy_binary_heap(heap);
}