#include "huffman.h"
#include "huffmantree.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*static const long BYTES_PER_READ = 5; for testing only*/
static const long BYTES_PER_READ = 1000;

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
	int k = 0;
	int counter = 0;
	char buffer = '\0';

	int garbage_zeros = 0;

	char *string;
	binary_heap* heap;
	node *n;

	/*read 1000 bytes per time*/
	heap = create_binary_heap(10);
	string = read_bytes_from_file(input, BYTES_PER_READ*k, BYTES_PER_READ);
	count_frequencies(heap, string);
	while (string[BYTES_PER_READ] == '\0') {
		k++;
		free(string);
		string = read_bytes_from_file(input, BYTES_PER_READ*k, BYTES_PER_READ);
		count_frequencies(heap, string);
	}
	add_to_heap(heap);
	n = build_tree(heap);
	build_char_code(n);
	FILE *fp = fopen(output, "w");

	/*output tree to file->output :
	* numberofchars charfreqEOFcharfreqEOF...
	* char is 1 byte, freq is 
	*/
	fputc(heap->index, fp);

	for (i = 0; i < heap->index; i++) {
		fputc(heap->characters[i], fp);
		fprintf(fp, "%lld %c", heap->frequencies[i], EOF); /*delimiter needs to be char that can't be in tree*/
	}

	free(string);

	/*output codes to file*/
	i = 0;
	counter = 0;
	k = 0;
	string = read_bytes_from_file(input, BYTES_PER_READ*k, BYTES_PER_READ);
	while (string[i] != '\0') {
		int j = 0;
		
		while (get_code(string[i])[j] != '\0') {
			if (counter == 8) {
				write_byte_to_file(fp, buffer);
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
		/*load next part if necessary*/
		if (string[i] == '\0') {
			k++;
			free(string);
			string = read_bytes_from_file(input, BYTES_PER_READ*k, BYTES_PER_READ);
			i = 0;
		}
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
	unsigned char code[256];
	int code_index = 0;
	int last_byte_read = 0; /*boolean value*/

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
	buffer = fgetc(fpi);

	while (!last_byte_read) { 
		int j = 0;
		char result;
		int offset = 0;

		if (ftell(fpi) == (input_length - 2)) { /*second last byte: byte with garbage zeros!*/
			offset = garbage_zeros; /*last 'offset' bits are garbage zeros*/
			last_byte_read = 1;
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
				code_index = 0;
			}
		}
		buffer = fgetc(fpi);
	}

	fclose(fpi);
	fclose(fpo);
	destroy_binary_heap(heap);
}