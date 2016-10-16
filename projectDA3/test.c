#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "huffmantree.h"


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
	binary_heap* heap;

	string = read_JSON_from_file("data.txt"); /*contains 0 12 34 567 890 (in JSON)*/
	heap = build_heap(string);
	assert(heap->size == 10);
	printf("char: %c, freq: %lld \n", heap->nodes[1]->character, heap->nodes[1]->frequency);
	assert(heap->nodes[1]->frequency == 1);
	assert(heap->nodes[2]->frequency == 1);
	assert(heap->nodes[3]->frequency == 1);
	assert(heap->nodes[4]->frequency == 1);
	assert(heap->nodes[5]->frequency == 1);
	assert(heap->nodes[6]->frequency == 1);
	assert(heap->nodes[7]->frequency == 1); 
	assert(heap->nodes[8]->frequency == 2); /*char 0 with freq 2*/ 
	assert(heap->nodes[9]->frequency == 1);
	assert(heap->nodes[10]->frequency == 1);

	destroy_binary_heap(heap);
	free(string);
}

void build_tree_test() {
	char *string;
	binary_heap* heap;
	node *n;

	string = read_JSON_from_file("data.txt"); /*contains 0 12 34 567 890 (in JSON)*/
	heap = build_heap(string);
	n = build_tree(heap);
	assert(n->left->left->character == '0');
	assert(n->left->right->left->character == '8');
	assert(n->left->right->right->character == '3');
	assert(n->right->left->left->character == '5');
	assert(n->right->left->right->left->character == '6');
	assert(n->right->left->right->right->character == '2');
	assert(n->right->right->left->left->character == '1');
	assert(n->right->right->left->right->character == '9');
	assert(n->right->right->right->left->character == '7');
	assert(n->right->right->right->right->character == '4');

	destroy_binary_heap(heap);
	free(string);
}

void run_tests() {
	/*add all tests here*/
	printf("running tests... \n");

	huffmanheap_test();
	build_heap_test();
	build_tree_test();

	printf("tests completed \n");
}