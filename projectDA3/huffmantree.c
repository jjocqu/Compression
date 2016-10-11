#include "huffmantree.h"

/*create and destroy functions*/
node* create_node(char character, long long frequency) {
	node *node;
	node->character = character;
	node->frequency = frequency;
	
	return node;
}

void destroy_node(node *node) {
	if (node->left) {
		destroy_node(node->left);
	}
	if (node->right) {
		destroy_node(node->right);
	}
	free(node);
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
void add_node(node *node) {

}

node* remove_min() {

}


void build_heap(const char* content) {

}

void build_tree() {

}