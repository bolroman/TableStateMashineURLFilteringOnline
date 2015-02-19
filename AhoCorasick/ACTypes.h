#ifndef ACTYPES_H_
#define ACTYPES_H_

#include "../Common/HashMap/HashMap.h"

struct st_node;

typedef struct {
	char c;
	struct st_node *ptr;
} Pair;

typedef struct st_node {
	int realID;
	int id;
	int numGotos;
	int match;
	int numMatches;
	char *message;
	int messageLength;
	HashMap *gotos;
	struct st_node *failure;
	int hasFailInto; // TRUE if some other node fails into this node
	int depth;
	char c1, c2;
	int isFirstLevelNode;
	int isSecondLevelNode;
	int marked;
	unsigned char* huffman[20];
	int huffman_len[20];
	char *patterns[20];
	int patternsLen[20];

} Node;

typedef struct {
	Node *root;
	unsigned int size;
} ACTree;

#endif /* ACTYPES_H_ */
