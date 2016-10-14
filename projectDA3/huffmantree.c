#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "huffmantree.h"


/*create and destroy functions*/
node* create_node(char character, long long frequency) {
	node *n = (node*) malloc(sizeof(node));
	n->character = character;
	n->frequency = frequency;
	n->left = NULL;
	n->right = NULL;
	
	return n;
}

void destroy_node(node *n) {
	if (n) {
		if (n->left) {
			destroy_node(n->left);
		}
		if (n->right) {
			destroy_node(n->right);
		}
		free(n);
		n = NULL;
	}
}

binary_heap* create_binary_heap(int size) {
	int i;
	binary_heap *heap = (binary_heap*) malloc(sizeof(heap));
	free(heap);
	heap->max_size = size;
	heap->size = 0;
	heap->nodes = (node**) malloc(sizeof(node*)*heap->max_size);

	for (i = 0; i < heap->max_size; i++) {
		heap->nodes[i] = NULL;
	}

	return heap;
}

void destroy_binary_heap(binary_heap *bheap) {
	int i;
	
	if (bheap) {
		if (bheap->nodes) {
			for (i = 0; i < bheap->max_size; i++) {
				destroy_node(bheap->nodes[i]);
			}

			free(bheap->nodes);
			bheap->nodes = NULL;
		}
		free(bheap);
		_CrtDumpMemoryLeaks();
		bheap = NULL;
	}
}

/*swap function*/
void swap_nodes(binary_heap *heap, node *first, node *second) {
	node *temp;

	temp = first;
	first = second;
	second = temp;
}


/*heap operations*/
void add_node(binary_heap *heap, node *n) {
	int pos = heap->size + 1; /*+1 because pos 0 isn't used!*/
	int pos_parent = pos / 2;
	
	if (pos == 1) { /*first node added*/
		heap->nodes[pos] = n;
	}
	else {
		if (heap->size > heap->max_size) { /*realloc more memory if necessary*/
			heap->max_size = heap->max_size * 2;
			heap->nodes = realloc(heap->nodes, sizeof(node*)*heap->max_size);
		}

		heap->nodes[pos] = n;

		while (heap->nodes[pos]->frequency < heap->nodes[pos_parent]->frequency) {
			swap_nodes(heap, heap->nodes[pos], heap->nodes[pos_parent]);
		}
	}

	heap->size++;
}

/*remove first node and put last in root and fix heap*/
node* remove_min(binary_heap *heap) {
	node *first;
	int pos = heap->size + 1;
	int pos_left = pos * 2;
	int pos_right = pos * 2 + 1;

	swap_nodes(heap, heap->nodes[1], heap->nodes[heap->size + 1]);
	destroy_node(heap->nodes[heap->size+1]);
	heap->nodes[heap->size + 1] = NULL;

	while (heap->nodes[pos]->frequency > heap->nodes[pos_left]->frequency ||
			heap->nodes[pos]->frequency > heap->nodes[pos_right]->frequency) {
		if (heap->nodes[pos_left]->frequency < heap->nodes[pos_right]->frequency) {
			swap_nodes(heap, heap->nodes[pos], heap->nodes[pos_left]);
		}
		else {
			swap_nodes(heap, heap->nodes[pos], heap->nodes[pos_right]);
		}
	}

	heap->size--;
	return first;
}


void build_heap(const char* content) {

}

void build_tree() {

}