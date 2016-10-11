#include "huffmantree.h"
#include <stdlib.h>

/*create and destroy functions*/
node* create_node(char character, long long frequency) {
	node *node;
	node->character = character;
	node->frequency = frequency;
	
	return node;
}

void destroy_node(node *n) {
	if (n->left) {
		destroy_node(n->left);
	}
	if (n->right) {
		destroy_node(n->right);
	}
	free(n);
}

binary_heap* create_binary_heap(int size) {
	binary_heap *heap;
	heap->max_size = size;
	heap->size = 0;
	heap->nodes = malloc(sizeof(node*)*heap->max_size);

	return heap;
}

void destroy_binary_heap(binary_heap *heap) {
	int i;

	for (i = 0; i < heap->max_size; i++) {
		destroy_node(heap->nodes[i]);
	}

	free(heap->nodes);
}

/*heap operations*/
void add_node(binary_heap *heap, node *n) {
	int pos = heap->size + 1; /*+1 because pos 0 isn't used!*/
	int pos_parent = pos / 2;
	node *temp;

	heap->nodes[pos] = n;

	while (heap->nodes[pos]->frequency < heap->nodes[pos_parent]->frequency) {
		temp = heap->nodes[pos];
		heap->nodes[pos] = heap->nodes[pos_parent];
		heap->nodes[pos_parent] = temp;
	}
}

node* remove_min(binary_heap *heap) {

}


void build_heap(const char* content) {

}

void build_tree() {

}