#include<iostream>
#include<queue>
using namespace std;
#include <stdint.h>

#define KEY_SIZE 4
#define VALUE_SIZE 4
#define NODE_POINTER_SIZE 4
#define BLOCK_SIZE 64
#define INTERNAL_NODE_ORDER  (BLOCK_SIZE - NODE_POINTER_SIZE + KEY_SIZE - sizeof(int) -sizeof(bool)) /(KEY_SIZE +  NODE_POINTER_SIZE) 
#define DATA_NODE_ORDER (BLOCK_SIZE - NODE_POINTER_SIZE + KEY_SIZE + VALUE_SIZE- sizeof(int) -sizeof(bool))/(KEY_SIZE + VALUE_SIZE)
#define ORDER min(INTERNAL_NODE_ORDER ,DATA_NODE_ORDER)

const int MAX_SIZE = 7;

struct node {
	/** union ValuesOrNodes
	*   store value if node is datanode else for internal node it will store pivotmap
	*/
	union ValuesOrNodes {
		int value[MAX_SIZE-1]; /**< sorted array of values corresponding to keys */
		node* pivotmap[MAX_SIZE]; /**< sorted array of node pointer corresponding to keys */
		//node* childnodes[MAX_SIZE]; /**< sorted array of node pointer corresponding to keys */
	} d;
	int   key[MAX_SIZE-1]; /**< sorted array of different keys */
	int  n; /**< number of keys present in node */
	bool  datanode; /**< datanode will tell us if node is datanode or internalnode */
	node* nextsib; /**< pointer to next sibling */
};

void display(node* root);
node* insert_node(node* root, int key,int value);
int delete_key(node* root,int key);
void search(node* root,int key);
void search_all(node* root);
void get_interval_values(node*root,int start_key,int end_key);
void invarient_b_plus_tree(node* root);
