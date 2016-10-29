#include "huffmantree.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void print_heap(binary_heap *heap) {
	int i;
	printf("printing heap: \n");
	for (i = 0; i < heap->max_size; i++) {
		if (heap->nodes[i]) {
			printf("pos: %d, char %c, freq: %lld \n", i, heap->nodes[i]->character, heap->nodes[i]->frequency);
		}
		else {
			printf("pos: %d, NULL \n", i);
		}
	}
}

/*create and destroy functions*/
node* create_node(char character, long long frequency) {
	node *n = (node*) malloc(sizeof(node));
	if (!n) {
		printf("error: mem alloc failed");
	}
	n->character = character;
	n->frequency = frequency;
	n->left = NULL;
	n->right = NULL;
	strcpy(n->code, "");
	
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
	if (!heap) {
		printf("error: mem alloc failed");
	}
	heap->max_size = size;
	heap->size = 0;
	heap->nodes = (node**) malloc(sizeof(node*)*heap->max_size);
	if (!heap->nodes) {
		printf("error: mem alloc failed");
	}

	for (i = 0; i < heap->max_size; i++) {
		heap->nodes[i] = NULL;
	}

	heap->char_freq_size = size;
	heap->characters = (char*) malloc(sizeof(char) * heap->max_size);
	heap->frequencies = (long long*) malloc(sizeof(long long) * heap->max_size);
	heap->index = 0;

	if (!heap->characters || !heap->frequencies) {
		printf("error: mem alloc failed");
	}

	/*empty char_code (only necessary if you run it a second time*/
	for (i = 0; i < 256; i++) {
		char_code[i][0] = '\0';
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
		if (bheap->characters) {
			free(bheap->characters);
		}
		if (bheap->frequencies) {
			free(bheap->frequencies);
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

	int i;
	
	if (pos == 1) { /*first node added*/
		heap->nodes[pos] = n;
	}
	else {
		if (heap->size+1 >= heap->max_size) { /*realloc more memory if necessary*/
			heap->max_size = heap->max_size * 2;
			heap->nodes = (node**) realloc(heap->nodes, sizeof(node*)*heap->max_size);
			if (!heap->nodes) {
				printf("error: mem alloc failed");
			}

			for (i = heap->size+1; i < heap->max_size; i++) {
				heap->nodes[i] = NULL;
			}
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

		while (!balanced) {
			/*check if balancing is necassary*/
			if (!(heap->nodes[pos_left]) || pos_left >= heap->max_size) { /*if there is no left child then there will be no right child*/
				break;
			}
			else if (!(heap->nodes[pos_right]) || pos_right >= heap->max_size) { /*only a left child*/
				if (heap->nodes[pos_left]->frequency < heap->nodes[pos]->frequency) {
					swap_nodes(heap, pos_left, pos);
					pos = pos_left;
					pos_left = pos * 2;
					pos_right = pos * 2 + 1;
				}
				else {
					break;
				}
			}
			else if (heap->nodes[pos_left] && heap->nodes[pos_right]) { /*a left and a right child*/
				if (heap->nodes[pos_left]->frequency <= heap->nodes[pos_right]->frequency) {
					min_pos = pos_left;
				}
				else {
					min_pos = pos_right;
				}
				if (heap->nodes[min_pos]->frequency < heap->nodes[pos]->frequency) {
					swap_nodes(heap, min_pos, pos);
					pos = min_pos; 
					pos_left = pos * 2;
					pos_right = pos * 2 + 1;
				}
				else {
					break;
				}
			}
		}
	}

	heap->size--;
	return first;
}

/*help function
* returns -1 if characters doesn't contains character
* returns the index if it does
*/
int contains_char(char *characters, char character, int max) {
	int i = 0;

	for (i = 0; i <= max; i++) {
		if (characters[i] == character) {
			return i;
		}
	}

	return -1;
}

void count_frequencies(binary_heap *heap, char *content) {
	int i = 0;

	while (content[i] != '\0') {
		int j = contains_char(heap->characters, content[i], heap->index);
		if (j == -1) {
			if (heap->index >= heap->char_freq_size) { /*realloc more memory if necessary*/
				heap->char_freq_size = 2 * heap->char_freq_size;
				heap->characters = (char*)realloc(heap->characters, sizeof(char) * heap->char_freq_size);
				heap->frequencies = (long long*)realloc(heap->frequencies, sizeof(long long) * heap->char_freq_size);
				if (!heap->characters || !heap->frequencies) {
					printf("error: mem alloc failed");
				}
			}
			heap->characters[heap->index] = content[i];
			heap->frequencies[heap->index] = 1;
			heap->index++;

		}
		else { /*characters[j] == content[i]*/
			heap->frequencies[j]++;
		}
		i++;
	}
}

void add_to_heap(binary_heap *heap) {
	int i;

	for (i = 0; i < heap->max_size; i++) {
		if (heap->characters[i] > 0 && heap->frequencies[i] > 0) {
			add_node(heap, create_node(heap->characters[i], heap->frequencies[i]));
		}
	}
}

node* build_tree(binary_heap *heap) {
	node *n1;
	node *n2;
	node *parent;

	while (heap->size != 1) { /*continue untill there is one node in heap*/
		n1 = remove_min(heap);
		n2 = remove_min(heap);
		parent = create_node(NULL, n1->frequency + n2->frequency);
		parent->left = n1;
		parent->right = n2;
		add_node(heap, parent);	
	}

	return parent;
}

void build_char_code(node *node) {
	if (node->left) {
		strcat(node->left->code, node->code);
		strcat(node->left->code, "0");
		build_char_code(node->left);
	}
	if (node->character > 0) { /*external node*/
		strcat(node->code, "\0");
		strcpy(char_code[node->character], node->code);
	}
	if (node->right) {
		strcat(node->right->code, node->code);
		strcat(node->right->code, "1");
		build_char_code(node->right);
	}
}

char* get_code(char character) {
	return char_code[character];
}

char get_char(node *n, char *code) {
	int i = 0;
	
	/*go to matching external node*/
	while (code[i] != '\0') {
		if (code[i] == '1') { /*go right*/
			n = n->right;
		}
		else { /*code[i] == 0*/
			n = n->left;
		}

		if (!n) { /*if new node is NULL, code isn't long enough yet (or doesn't exist)*/
			return '\0';
		}

		i++;
	}
	
	return n->character;
}