#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

typedef struct node {
	char character;
	node *left;
	node *right;
};

typedef struct list {
	int alphabet_size;
	long long *frequencies;
	node **root;
};

void create_node(char character);
void destroy_node();
void create_list();
void destroy_list();

/*count the frequencies and initialize list*/
void build_list(const char* content);
/*build huffman tree using the list*/
void build_tree();
/*TODO functions to read codes from huffman tree*/
#endif