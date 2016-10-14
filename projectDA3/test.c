#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "huffmantree.h"


void huffmanheap_test() {
	binary_heap *heap;
	node *node0;

	node0 = create_node('0', 1);
	heap = create_binary_heap(10);
	add_node(heap, node0);
	assert(heap->size == 1);
	add_node(heap, create_node('1', 2));
	assert(heap->size == 2);

	destroy_binary_heap(heap);
}

void run_tests() {
	/*add all tests here*/
	printf("running tests... \n");
	huffmanheap_test();
	printf("tests completed \n");
}