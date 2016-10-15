#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

/* if node is internal: 
*		- character is NULL
*		- frequency is sum of direct children
*/
typedef struct {
	char character;
	struct node *left;
	struct node *right;
	long long frequency;
} node;

/*an array is used to store the binary heap
* no element at position 0
*/
typedef struct {
	int size;
	int max_size;
	node **nodes;
} binary_heap;

/*create and destroy functions*/
node* create_node(char character, long long frequency);
void destroy_node(node *n);
binary_heap* create_binary_heap(int size);
void destroy_binary_heap(binary_heap *heap);

/*heap operations*/
void add_node(binary_heap *heap, node *n);
node* remove_min(binary_heap *heap);

/*count the frequencies and add them to heap*/
binary_heap* build_heap(const char* content);
/*build huffman tree using the heap*/
void build_tree(binary_heap *heap);

/*TODO functions to read codes from huffman tree*/


#endif