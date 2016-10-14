#include "huffmantree.h"

#include <stdlib.h>
#include <stdio.h>

void print_heap(binary_heap *heap) {
	int i;
	printf("printing heap: \n");
	for (i = 0; i < heap->max_size; i++) {
		if (heap->nodes[i]) {
			printf("pos: %d, char %c, freq: %d \n", i, heap->nodes[i]->character, heap->nodes[i]->frequency);
		}
		else {
			printf("pos: %d, NULL \n", i);
		}
	}
}

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
	binary_heap *heap = (binary_heap*) malloc(sizeof(binary_heap));
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
		bheap = NULL;
	}
}

/*swap function*/
void swap_nodes(binary_heap *heap, int pos_first, int pos_second) {
	node *temp;

	temp = heap->nodes[pos_first];
	heap->nodes[pos_first] = heap->nodes[pos_second];
	heap->nodes[pos_second] = temp;
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
			swap_nodes(heap, pos, pos_parent);
			pos = pos_parent;
			pos_parent = pos / 2;

			if (pos == 1) {
				break;
			}
		}
	}

	heap->size++;
}

/*remove first node and put last in root and fix heap*/
node* remove_min(binary_heap *heap) {
	node *first;
	int pos = 1; /*pos of smallest node*/
	int pos_left = pos * 2;
	int pos_right = pos * 2 + 1;
	int min_pos;
	int balanced = 0; /*boolean variable*/

	/*heap is only 1 node*/
	if (heap->size == 1) {
		first = heap->nodes[1];
		heap->nodes[1] = NULL;
	}
	else {
		swap_nodes(heap, 1, heap->size);
		first = heap->nodes[heap->size];
		heap->nodes[heap->size] = NULL;

		//print_heap(heap);

		while (!balanced) {
			/*check if balancing is necassary*/
			if (!heap->nodes[pos_left]) { /*if there is no left child then there will be no right child*/
				break;
			}
			else if (!heap->nodes[pos_right]) { /*only a left child*/
				if (heap->nodes[pos_left]->frequency < heap->nodes[pos]->frequency) {
					swap_nodes(heap, pos_left, pos);
				}
			}
			else { /*a left and a right child*/
				if (heap->nodes[pos_left]->frequency <= heap->nodes[pos_right]->frequency) {
					min_pos = pos_left;
				}
				else {
					min_pos = pos_right;
				}
				if (heap->nodes[min_pos]->frequency < heap->nodes[pos]->frequency) {
					swap_nodes(heap, min_pos, pos);
				}
			}
			/*go one 'level' higher in tree*/
			pos = pos / 2;
			pos_left = pos * 2;
			pos_right = pos * 2 + 1;
		}
	}

	heap->size--;
	return first;
}


void build_heap(const char* content) {
	printf("%s", content);
}

void build_tree() {

}