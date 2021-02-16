#include<iostream>
#include<map>
#include<queue>
using namespace std;

const int MAX_SIZE = 5;

struct node
{
	int key[MAX_SIZE];
	//node* pivotmap[MAX_SIZE+1];
	union Data
	{
		int value[MAX_SIZE];
		node* pivotmap[MAX_SIZE+1];
	} d;
	int n;
	bool datanode;
	node* nextsib; //pointer to next sibling
};


void display(node* root);
node* split_internalnode(node* root,node* temp);
node* split(node* root, int key,int value);
int insert(node* root, int key,int value);
node* insert_node(node* root, int key,int value);
void delete_key(node* root, int key);
void search(node* root,int key);
void search_all(node* root);
void get_interval_values(node*root, int start_key, int end_key);
//void search_interval(node* root, int start_i, int end_i);