#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "huffmantree.h"
#include "huffman.h"
#include "common.h"


void huffmanheap_test() {
	binary_heap *heap;
	node *min;
	node *min2;

	/*remove 1*/
	heap = create_binary_heap(10);
	add_node(heap, create_node('0', 1));
	assert(heap->size == 1);
	add_node(heap, create_node('1', 2));
	assert(heap->size == 2);
	min = remove_min(heap);
	assert(heap->size == 1);
	assert(heap->nodes[1]->frequency == 2);

	destroy_binary_heap(heap);
	destroy_node(min);

	/*remove 2*/
	heap = create_binary_heap(10);
	add_node(heap, create_node('0', 1));
	add_node(heap, create_node('1', 2));
	add_node(heap, create_node('2', 3));
	assert(heap->size == 3);
	min = remove_min(heap);
	assert(heap->size == 2);
	assert(heap->nodes[1]->frequency == 2);
	assert(heap->nodes[2]->frequency == 3);

	destroy_binary_heap(heap);
	destroy_node(min);

	/*remove 3*/
	heap = create_binary_heap(10);
	add_node(heap, create_node('0', 1));
	assert(heap->size == 1);
	min = remove_min(heap);
	assert(heap->size == 0);
	assert(heap->nodes[1] == NULL);

	destroy_binary_heap(heap);
	destroy_node(min);

	/*remove 4*/
	heap = create_binary_heap(10);
	add_node(heap, create_node('0', 1));
	add_node(heap, create_node('1', 2));
	add_node(heap, create_node('2', 3));
	add_node(heap, create_node('3', 4));
	assert(heap->size == 4);
	min = remove_min(heap);
	assert(heap->size == 3);
	assert(heap->nodes[1]->frequency == 2);
	assert(heap->nodes[2]->frequency == 4);
	assert(heap->nodes[3]->frequency == 3);
	min2 = remove_min(heap);
	assert(heap->size == 2);
	assert(heap->nodes[1]->frequency == 3);
	assert(heap->nodes[2]->frequency == 4);
	
	destroy_binary_heap(heap);
	destroy_node(min);
	destroy_node(min2);

	/*build heap*/
	heap = create_binary_heap(10);
	add_node(heap, create_node('5', 5));
	add_node(heap, create_node('4', 4));
	add_node(heap, create_node('3', 3));
	add_node(heap, create_node('2', 2));
	add_node(heap, create_node('1', 1));
	assert(heap->size == 5);
	assert(heap->nodes[1]->frequency == 1);
	assert(heap->nodes[2]->frequency == 2);
	assert(heap->nodes[3]->frequency == 4);
	assert(heap->nodes[4]->frequency == 5);
	assert(heap->nodes[5]->frequency == 3);

	destroy_binary_heap(heap);
}

void build_heap_test() {
	char *string;
	char *string2;
	binary_heap* heap;

	FILE *fp = fopen("data.txt", "rb");
	string = read_bytes_from_file(fp, 0, 5); /*0,12,*/
	string2 = read_bytes_from_file(fp, 5, 1000);/*34,567,890*/
	heap = create_binary_heap(10);
	count_frequencies(heap, string); 
	count_frequencies(heap, string2);
	add_to_heap(heap);
	assert(heap->size == 11);
	assert(heap->nodes[1]->frequency == 1);
	assert(heap->nodes[2]->frequency == 1);
	assert(heap->nodes[3]->frequency == 1);
	assert(heap->nodes[4]->frequency == 1);
	assert(heap->nodes[5]->frequency == 1);
	assert(heap->nodes[6]->frequency == 2); /*char 0 with freq 2*/ 
	assert(heap->nodes[7]->frequency == 1); 
	assert(heap->nodes[8]->frequency == 4); /*char ',' with freq 4*/ 
	assert(heap->nodes[9]->frequency == 1);
	assert(heap->nodes[10]->frequency == 1);
	assert(heap->nodes[11]->frequency == 1);

	destroy_binary_heap(heap);
	free(string);
	free(string2);
	fclose(fp);
}

void build_tree_test() {
	char *string;
	binary_heap* heap;
	node *n;

	FILE *fp = fopen("data.txt", "rb");
	string = read_bytes_from_file(fp, 0, 1000); /*contains 0,12,34,567,890 */
	heap = create_binary_heap(10);
	count_frequencies(heap, string);
	add_to_heap(heap);
	n = build_tree(heap);
	assert(n->left->left->left->character == '5');
	assert(n->left->left->right->left->character == '3');
	assert(n->left->left->right->right->character == '4');
	assert(n->left->right->left->left->character == '8');
	assert(n->left->right->left->right->character == '2');
	assert(n->left->right->right->character == '0');
	assert(n->right->left->left->left->character == '1');
	assert(n->right->left->left->right->character == '9');
	assert(n->right->left->right->left->character == '6');
	assert(n->right->left->right->right->character == '7');
	assert(n->right->right->character == ',');
	
	destroy_binary_heap(heap);
	free(string);
	fclose(fp);
}

void char_code_test() {
	char *string;
	binary_heap* heap;
	node *n;

	FILE *fp = fopen("data.txt", "rb");
	string = read_bytes_from_file(fp, 0, 1000); /*contains 0,12,34,567,890*/
	heap = create_binary_heap(10);
	count_frequencies(heap, string);
	add_to_heap(heap);	
	n = build_tree(heap);
	build_char_code(n);
	assert(strcmp(get_code('0'), "011") == 0);
	assert(strcmp(get_code(','), "11") == 0);
	assert(strcmp(get_code('1'), "1000") == 0);
	assert(strcmp(get_code('2'), "0101") == 0);
	assert(strcmp(get_code('3'), "0010") == 0);
	assert(strcmp(get_code('4'), "0011") == 0);
	assert(strcmp(get_code('5'), "000") == 0);
	assert(strcmp(get_code('6'), "1010") == 0);
	assert(strcmp(get_code('7'), "1011") == 0);
	assert(strcmp(get_code('8'), "0100") == 0);
	assert(strcmp(get_code('9'), "1001") == 0);
	assert(strcmp(get_code('A'), "") == 0);

	destroy_binary_heap(heap);
	free(string);
	fclose(fp);
}

void encode_decode_test() {
	char char1;
	char char2;

	/*test 1*/
	encode("data.txt", "compressed"); /*contains 0,12,34,567,890*/
	decode("compressed", "reconstructed.txt");

	FILE *fp1 = fopen("data.txt", "r");
	FILE *fp2 = fopen("reconstructed.txt", "r");

	while ((char1 = fgetc(fp1)) != EOF) { /*check if all characters match*/
		char2 = fgetc(fp2);
		assert(char1 == char2);
	}

	fclose(fp1);
	fclose(fp2);
}

void run_tests() {
	/*add all tests here*/

	FILE *fp = fopen("data.txt", "w");
	fprintf(fp, "0,12,34,567,890");
	fclose(fp);

	printf("running tests... \n");

	huffmanheap_test();
	build_heap_test();
	build_tree_test();
	char_code_test();
	
	encode_decode_test(); 

	fp = fopen("data.txt", "w");
	fprintf(fp, "Hallo ik ben Jens Jocque en dit is een test. \n Hier nog wat cijfers 123, 6578, 456782");
	fclose(fp);

	encode_decode_test();

	printf("tests completed \n");
}