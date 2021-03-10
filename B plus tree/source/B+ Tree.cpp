#include "B+ Tree.h"
#include<fstream>
ofstream logFile(LOG);
ofstream errorlogFile(ERRORLOG);

/**
* display() is a function for displaying created B-plus tree in breadth first traversal
*
* This will print keys, pivots or value associated with node
*
* @param root pointer to root of B plus tree
*/
void display(node* root)
{
	if (root == NULL) {
		cout<<"Display : Tree is Empty"<<endl;
		logFile<<"Display : Tree is Empty"<<endl;
		return;
	}
	queue<node*> g;
	g.push(root);
	while (!g.empty()) {
		node *n = g.front();
		g.pop();
		cout<<"datanode : "<<n->datanode<<"  total keys :"<<n->n<<" keys: ";
		for (int i = 0; i < n->n; i++) {
			cout<<n->key[i]<<"  ";
		}
		if (n->datanode) {
			cout<<" values : ";
			for(int i = 0; i < n->n; i++)
			{
				cout<<n->d.value[i]<<"  ";
			}

		} else {
			cout<<"PIVOTS :";
			if(!n->datanode) {
				for (int i = 0; i<n->n+1; i++) { 
					g.push(n->d.pivotmap[i]);
				}
			}
		}
		cout<<endl;
	}
	cout <<endl;
}

/**
* If overflow can occure at internal node, split_internalnode() gets called which will split an node into two 
* and return middle elements which needs to get adjusted or merged at parent node 
* 
* Overflow can occure as new key and new sibling of one of the child node needs to be added at parent node 
* Algorithm : new node will get created and elements present in an current node and key, new sibling which needs to be added in current node 
* will get distributed equally between current node and newly created node 
*
* @param root is pointer node which needs to get split
* @param key is a key which needs to get inserted to the node
* @param piv_index is index at which an child_node is present which also points to the new sibling node 
*/
int split_internal_node(node* root, int key, int piv_index)
{
	int half_maxsize = (MAX_SIZE-1)%2? (MAX_SIZE-1)/2 +1 : (MAX_SIZE-1)/2; /** half of total elements */

	node* n2 = new node;
	n2->datanode = 0;
	n2->nextsib = NULL;

	int pivcount_n, ncount_n, ncount_r,midkey;

	/** 3 cases can be occure 
	* if piv_index == half_maxsize => new key will become middle key and sibling needs to get added as 1st pivot pointer for n2
	* if piv_index < half_maxsize  => new key and sibling needs to get added at root/current node , therefore right shifting is done 
	*                              until half_maxsize to make array sorted as new elemennt get added, then rest of the elements get added to n2
	*  if piv_index > half_maxsize => new key and sibling needs to get added at n2, so, simply root/current node element,sibling ,key
	*                              needs to get added in sorted order
	*/

	if (half_maxsize == piv_index) {
		midkey = key;
		n2->d.pivotmap[0] = root->d.pivotmap[piv_index]->nextsib;
		ncount_n = 0;
		pivcount_n = 1;
		ncount_r = piv_index;
	} else if (piv_index < half_maxsize) {
		int temp_key;
		node* temp_node = NULL;
		int key_ = key;
		node* node_ = root->d.pivotmap[piv_index]->nextsib;
		int i;
		for (i = piv_index; i < half_maxsize; i++) {
			temp_key = root->key[i];
			temp_node = root->d.pivotmap[i+1];

			root->key[i] = key_;
			root->d.pivotmap[i+1] = node_; 

			key_ = temp_key;
			node_ = temp_node;

		}

		midkey = key_;
		n2->d.pivotmap[0] = node_;
		ncount_n = 0;
		pivcount_n = 1;
		ncount_r =  i; 
	} else {
		midkey = root->key[half_maxsize];
		ncount_n = 0;
		pivcount_n = 0;
		for (int i = half_maxsize+1; i < piv_index; i++) {
			n2->key[ncount_n++] = root->key[i];
			n2->d.pivotmap[pivcount_n++] = root->d.pivotmap[i];
		}
		n2->key[ncount_n++] = key;
		n2->d.pivotmap[pivcount_n++] = root->d.pivotmap[piv_index];
		n2->d.pivotmap[pivcount_n++] = root->d.pivotmap[piv_index]->nextsib;

		ncount_r = piv_index;
	}
	/** adding remaining elemnts from node to newly created node */
	for (int j = ncount_r; j < root->n; j++) {
		n2->key[ncount_n] = root->key[j];
		n2->d.pivotmap[ncount_n+1] = root->d.pivotmap[j+1];
		ncount_n++;
	}
	/** update value of n for root and n2 and sibling for root */
	root->n = half_maxsize;
	n2->n = ncount_n;
	root->nextsib = n2;
	return midkey;
}


/**
* If overflow can occure at data node due to addition of key,value, split_() gets called which will split a datanode node into two 
* and return middle elements which needs to get adjusted or merged at parent node 
* 
* Algorithm : new datanode node will get created and elements present in an current node and new key, value which needs to be added in current node 
* will get distributed equally between current node and newly created node 
*
* @param root is pointer node which needs to get split
* @param key is a key which needs to get inserted to the node
* @param value is a value which needs to get inserted to the node
*/
int split_data_node(node* root, int key, int value)
{
	int half_maxsize = MAX_SIZE%2? MAX_SIZE/2 +1 : MAX_SIZE/2; /** half of total elements */

	node* n2 = new node;
	n2->datanode = 1;

	int i,count_n, count_r;
	int flag = 0; /** it will tell whether new key, value has been added or not : if flag =0 : key,value not added yet*/
	/** 2 cases can be occure 
	* if key needs to get added at root/current node => flag =1 ,right shifting is done until half_maxsize to make array sorted as new elemennt get added, 
	*                                                   then rest of the elements get added to n2 without performing any comparison
	* if key needs to get added at n2 node => flag remains 0, so comparisons with new key needs to be done and new key and sibling needs to get added 
	*                                          at n2 accordingly
	*/


	for (i = 0; i < half_maxsize; i++) {
		//if(key<root->key[i]){//key already present; return -1;}else
		if (key == root->key[i]) {
			//cout<<"Given key is already present";
			//logFile<<"Given key, "<<key<<" is already present"<<endl;
			errorlogFile<<"Given key, "<<key<<" is already present"<<endl;
			return -1;
		}
		if (key < root->key[i]) {
			int temp_key,temp_value;
			for(; i < half_maxsize; i++)
			{
				temp_key=root->key[i];
				temp_value = root->d.value[i];

				root->key[i] = key;
				root->d.value[i] = value;

				key = temp_key;
				value = temp_value;
			}
			flag = 1;
			break;
		}
	}

	if (flag) {
		n2->key[0] = key;
		n2->d.value[0] = value;
		count_n = 1;
		count_r = i;
	} else {
		count_n = 0;
		for (; i < root->n; i++) {
			if (key == root->key[i]) {
				//cout<<"Given key is already present";
				//logFile<<"Given key, "<<key<<" is already present"<<endl;
				errorlogFile<<"Given key, "<<key<<" is already present"<<endl;
				//delete n2 and make it null
				delete n2;
				n2 = NULL;
				return -1;
			}
			if (key < root->key[i]) {
				n2->key[count_n] = key;
				n2->d.value[count_n] = value;
				count_n ++;

				count_r = i;
				break;

			} else {
				n2->key[count_n] = root->key[i];
				n2->d.value[count_n] = root->d.value[i];
				count_n++;
			}
		}

		if (i == root->n) {// when new elemet needs to be inserted at end
			n2->key[count_n] = key;
			n2->d.value[count_n] = value;
			count_n++;
		}

	}

	/** adding remaining elemnts from node to newly created node */
	for (; i < root->n; i++) {
		n2->key[count_n] = root->key[i];
		n2->d.value[count_n] = root->d.value[i];
		count_n++;
	}

	/** update value of n for root and n2 and sibling for root */
	root->n = half_maxsize ;
	n2->n = count_n;
	n2->nextsib = root->nextsib;
	root->nextsib = n2;

	logFile<<"Given key, "<<key<<" is inserted successfully."<<endl;
	return n2->key[0];
}

/**
* insert() will insert the key and value at data node to appropriate loacation 
* and it will call function for spliting datanode or internal node if required
* 
* Algorithm : if root is datanode => if oveflow can occure call split_() function else simply add new key, value to that datanode in sorted manner
*             else root node is internal node =>  find appropriate pivot and call insert function recursively , 
*                                                 if return value is other than -1: it states that new key which is returned needs to be added at current node
*                                                      if overflow can occur at internal node by adding new key, call split_internalnode_ function
*                                                      else simply add new key with current node
*                                                 else i.e return value == -1 => no extra key needs to be added
*                                                    
* @param root is pointer node at which key, value needs to get added
* @param key is a key which needs to get inserted to the node
* @param value is a value which needs to get inserted to the node
*/
int insert(node* root, int key, int value)
{
	/* if node is internal-indexed node*/
	if (!root->datanode) {
		/* find appropriate pivot */
		int piv_index;
		for (piv_index = 0; piv_index < root->n; piv_index++) {
			if (key < root->key[piv_index]) {
				break;
			}
		}
		int prev = root->d.pivotmap[piv_index]->n;
		bool prev_datanode = root->d.pivotmap[piv_index]->datanode;
		int temp = insert(root->d.pivotmap[piv_index], key, value);

		if (temp != -1) {
			if(root->n == MAX_SIZE-1) { 
				/*if overflow can occure*/
				return split_internal_node(root, temp, piv_index);
			} else {
				for (int j = root->n; j > piv_index; j--) {
					root->key[j] = root->key[j-1];
				}
				root->key[piv_index]=temp;
				for (int j = root->n+1; j > piv_index+1; j--) {
					root->d.pivotmap[j] = root->d.pivotmap[j-1];
				}
				root->d.pivotmap[piv_index+1] = root->d.pivotmap[piv_index]->nextsib;

				root->n++;

				return -1;
			}
		}
		return -1;
	} else {
		/* node is data node/leaf node */
		if (root->n == MAX_SIZE-1) {
			/*if datanode can become overflow */
			return split_data_node(root,key,value);
		} else {
			/*add key to appropriate location */
			int j;
			for (j = 0; j < root->n; j++) {
				if(key == root->key[j])
				{
					//cout<<"Given key is already present"<<endl;
					//logFile<<"Given key, "<<key<<" is already present"<<endl;
					errorlogFile<<"Given key, "<<key<<" is already present"<<endl;
					return -1;
				}
				if(key < root->key[j]) {
					break;
				}
			}
			for (int k = root->n; k > j; k--) {
				root->key[k] = root->key[k-1];
				root->d.value[k] = root->d.value[k-1];
			}
			root->key[j] = key;
			root->d.value[j] = value;
			root->n++;
			logFile<<"Given key, "<<key<<" is inserted successfully."<<endl;
		}
	}
	return -1;
}

/**
* insert_node() will call insert() function and create new root if required 
* 
* Algorithm : if tree is empty => new root node will get created and key, value get added to that node
*             else insert function get called
*                  if return value is other than -1: it states that new root needs to be created and 
*                                                     new key which is returned needs to be added to that node
*                                                 else i.e return value == -1 => no new root require
*                                                    
* @param root is pointer node at which key, value needs to get added
* @param key is a key which needs to get inserted to the node
* @param value is a value which needs to get inserted to the node
*/
node* insert_node(node* root, int key, int value) //insert_node() create new root node if required
{
	if (root == NULL) {
		root = new node;
		root->datanode = 1;
		root->key[0] = key;
		root->d.value[0] = value;
		root->n = 1;
		root->nextsib = NULL;

	} else {
		int result = insert(root,key,value);
		if(result != -1) {
			node *new_node = new node;
			new_node->key[0] = result;
			/*update pivot pointers*/
			new_node->d.pivotmap[0] = root;
			new_node->d.pivotmap[1] = root->nextsib;
			new_node->n = 1;
			new_node->datanode = 0;
			new_node->nextsib = NULL;
			root = new_node;
		}
	}
	return root;
}

/**
* merge_nodes() will merge two internal nodes or data nodes
* 
* Algorithm : 1.assign left_child, right_child => if piv_index !=0(i.e. child node is not the first child of parent and left sibling for the child is present)
*                                                        left_child = pivotmap[piv_index-1],  right_child = pivotmap[piv_index]
*                                                   else left_child = pivotmap[piv_index],  right_child = pivotmap[piv_index+1]
*             2. Merge all elements (key, value or pivot) from right_child  to left_child
*                NOTE: while merging we have to repeatatively check if it is data node or internal node and merge values or pivots respectively
*                                                    
* @param root is pointer to parent node of children which are going to be merged
* @param piv_index is index to child which need to get mergerd with one of the sibling
*/
void merge_nodes(node* root,  int piv_index)
{
	bool datanode = root->d.pivotmap[piv_index]->datanode;
	/*leftchild is one in which we are going to merge content from right child*/
	node *left_child = NULL, *right_child = NULL; 
	int j;/*index from which pivot shifting is needed*/

	/*if left sibling is present merge with left sibling */
	if (piv_index != 0) {
		left_child = root->d.pivotmap[piv_index-1];
		right_child = root->d.pivotmap[piv_index];
		j = piv_index-1;
	} else {
		/*merge with right sibling */
		left_child = root->d.pivotmap[piv_index];
		right_child = root->d.pivotmap[piv_index+1];
		j = piv_index;
	}
	int left_child_n = left_child ->n;
	int right_child_n = right_child->n;
	if (!datanode) {
		left_child->key[left_child_n] = root->key[j];
		left_child_n++;
	}
	j++;
	int k;
	for (k = 0; k < right_child_n; k++) {
		left_child->key[left_child_n] = right_child->key[k];
		if(datanode) {
			left_child->d.value[left_child_n] = right_child->d.value[k] ;

		} else {
			left_child->d.pivotmap[left_child_n] = right_child->d.pivotmap[k];
		}
		left_child_n++;
	}
	if (!datanode) {
		left_child->d.pivotmap[left_child_n] = right_child->d.pivotmap[k];
	}
	left_child->n = left_child_n;

	/*shift pivot pointers of root to left as one of the child got merged with one of the siblings*/
	for (; j < root->n; j++) {
		root->key[j-1] = root->key[j];
		root->d.pivotmap[j] = root->d.pivotmap[j+1];
	}
	//delete /null the last child of root
	//root->d.pivotmap[j+1] = NULL;
	/*delete root->d.pivotmap[j+1];
	root->d.pivotmap[j+1] = NULL;*/
	if (datanode) {
		/*update nextsib pointer*/
		left_child->nextsib = right_child->nextsib;
	}
	root->n--;

	root->d.pivotmap[j] = NULL;
	//delete temp_to_be_deleted ;
	//temp_to_be_deleted = NULL;
	delete  right_child;
	right_child = NULL;

}


/**
* merge_internal_nodes() will merge two internal nodes
* 
* Algorithm : 1.assign left_child, right_child => if piv_index !=0(i.e. child node is not the first child of parent and left sibling for the child is present)
*                                                        left_child = pivotmap[piv_index-1],  right_child = pivotmap[piv_index]
*                                                   else left_child = pivotmap[piv_index],  right_child = pivotmap[piv_index+1]
*             2. Merge all elements (key, pivot) from right_child  to left_child
*                                                    
* @param root is pointer to parent node of children which are going to be merged
* @param piv_index is index to child which need to get mergerd with one of the sibling
*/
void merge_internal_nodes(node* root,  int piv_index)
{
	/* leftchild is one in which we are going to merge content from right child */
	node* left_child = NULL, *right_child = NULL;
	int j;/*index from which pivot shifting is needed*/
	/* if left sibling is present */
	if (piv_index != 0) {/* merge with left sibling */
		left_child = root->d.pivotmap[piv_index-1];
		right_child = root->d.pivotmap[piv_index];
		j = piv_index-1;
	} else {/*merge with right sibling */
		left_child = root->d.pivotmap[piv_index];
		right_child = root->d.pivotmap[piv_index+1];
		j = piv_index;
	}
	int left_child_n = left_child ->n;
	int right_child_n = right_child->n;
	left_child->key[left_child_n] = root->key[j];
	j++;
	int k;
	left_child_n++;
	for (k = 0; k < right_child_n; k++)
	{
		left_child->d.pivotmap[left_child_n] = right_child->d.pivotmap[k];
		left_child->key[left_child_n] = right_child->key[k];
		left_child_n++;
	}
	left_child->d.pivotmap[left_child_n] = right_child->d.pivotmap[k];
	left_child->n = left_child_n;

	/*shift pivot pointers of root to left as one of the child got merged with one of the siblings*/
	//node* temp_to_be_deleted = root->d.pivotmap[j];
	for (;j < root->n; j++)
	{
		root->key[j-1] = root->key[j];
		root->d.pivotmap[j] = root->d.pivotmap[j+1];
	}
	root->d.pivotmap[j] = NULL;
	delete  right_child;
	right_child = NULL;

	root->n--;
}


/**
* adjust_with_left_sibling() function will get call when underflow occur at internal node and 
* balancing needs to be done with the help of left sibling. If left sibling contains enough elements, 
* some of the elements can be borrowed from the left sibling to make distribution of elements equally
* and to balance the underflowed node
*
* Algorithm : 1. find the number of element which needs to be added in current child i.e. value of remain
*             2. right shift elements in  current child by value of remain to make vacant place at begining of current child
*             3. move (n=remain) elements from left sibling to current child
*
* @param root is pointer to parent node of child which need to be adjusted or balanced
* @param piv_index is index to child which need to get adjusted or balanced with the help of one of the sibling
*/
void adjust_with_left_sibling(node* root, int piv_index)
{
	int child_n = root->d.pivotmap[piv_index]->n;
	int sib_n = root->d.pivotmap[piv_index-1]->n;
	int temp_midkey = root->key[piv_index-1];
	int total = sib_n + child_n; //int64_t
	int remain = total - total / 2 - child_n;
	int k;
	/* right shift elements in  current child by value of remain */
	for (k = child_n-1; k >= 0; k--) {
		root->d.pivotmap[piv_index]->key[k+remain] = root->d.pivotmap[piv_index]->key[k];
		root->d.pivotmap[piv_index]->d.pivotmap[k+remain+1] = root->d.pivotmap[piv_index]->d.pivotmap[k+1];
	}
	root->d.pivotmap[piv_index]->d.pivotmap[k+remain+1] = root->d.pivotmap[piv_index]->d.pivotmap[k+1];
	root->d.pivotmap[piv_index]->n = child_n+remain;

	/* move (n=remain) elements from left sibling to current child */
	for (int k_ = remain; k_ > 0; k_--) {
		root->d.pivotmap[piv_index]->key[k+k_] = temp_midkey;
		temp_midkey = root->d.pivotmap[piv_index-1]->key[sib_n-1];
		root->d.pivotmap[piv_index]->d.pivotmap[k+k_] = root->d.pivotmap[piv_index-1]->d.pivotmap[sib_n];
		sib_n--;
	}
	root->key[piv_index-1] = temp_midkey;
	root->d.pivotmap[piv_index-1]->n = sib_n;
}

/**
* adjust_with_right_sibling() function will get call when underflow occur at internal node and 
* balancing needs to be done with the help of right sibling. If right sibling contains enough elements, 
* some of the elements can be borrowed from the right sibling to make distribution of elements equally
* and to balance the underflowed node
*
* Algorithm : 1. find the number of element which needs to be added in current child i.e. value of remain
*             2. move  (n=remain) elements from right sibling to current child
*             3. left shift elements in right sibling by value of remain to fill the created vacant place at begining of right child
*             
*
* @param root is pointer to parent node of child which need to be adjusted or balanced
* @param piv_index is index to child which need to get adjusted or balanced with the help of one of the sibling
*/
void adjust_with_right_sibling(node* root, int piv_index)
{
	int child_n = root->d.pivotmap[piv_index]->n;
	int sib_n = root->d.pivotmap[piv_index+1]->n;
	int temp_midkey = root->key[piv_index];
	int total = child_n + sib_n ; //int64_t
	int remain = total - total /2 -child_n;
	/* move (n=remain) elements from right sibling to current child */
	for (int k = 0; k < remain; k++) {
		root->d.pivotmap[piv_index]->key[child_n++] = temp_midkey;
		temp_midkey = root->d.pivotmap[piv_index+1]->key[k];
		root->d.pivotmap[piv_index]->d.pivotmap[child_n] = root->d.pivotmap[piv_index+1]->d.pivotmap[k];
	}
	root->key[piv_index] = temp_midkey;
	root->d.pivotmap[piv_index]->n = child_n;
	int k;
	/* left shift elements in right sibling by value of remain */
	for (k = remain; k < sib_n; k++) {
		root->d.pivotmap[piv_index+1]->key[k-remain] = root->d.pivotmap[piv_index+1]->key[k];
		root->d.pivotmap[piv_index+1]->d.pivotmap[k-remain] = root->d.pivotmap[piv_index+1]->d.pivotmap[k];
	}
	root->d.pivotmap[piv_index+1]->d.pivotmap[k-remain] = root->d.pivotmap[piv_index+1]->d.pivotmap[k];
	root->d.pivotmap[piv_index+1]->n = root->d.pivotmap[piv_index+1]->n-remain;
}

/**
* balance_internal_node() function will get call when underflow occur at internal node
* This function will balance the node by making adjustment (i.e borrowing some elements from sibling) or merging with the sibling
* Priority is given to adjustment with the sibling than merging as  merge can be costly than adjusting
* Condition for merging OR adjusment: if after merging two nodes, overflow may occure, adjument needs to be done else merging can be done
*
* Algorithm : 1. If left sibling doesn't exist, check right sibling , call function from merging or adjustment according to condition
*             2. else if right sibling doesn't exist, check left sibling , call function from merging or adjustment according to condition 
*             3. else (i.e. both siblings exists) , check if adjustement can be done with the help of either of sibling else merge with any of siblings
*
* @param root is pointer to parent node of child which need to be balanced
* @param piv_index is index to child which need to get balanced with the help of one of the sibling
*/
void balance_internal_node(node* root,int piv_index)
{
	/* Priority given to adjust than merging as  merge can be costly than adjusting*/
	if (piv_index == 0) {
		if(root->d.pivotmap[piv_index]->n+1+root->d.pivotmap[piv_index+1]->n < MAX_SIZE) {
			merge_internal_nodes( root,  piv_index);
			//merge_nodes(root,piv_index);
		} else {
			adjust_with_right_sibling(root, piv_index);
		}
	} else if (piv_index == root->n) {
		if (root->d.pivotmap[piv_index]->n+1+root->d.pivotmap[piv_index-1]->n < MAX_SIZE) {
			merge_internal_nodes( root, piv_index);
			//merge_nodes(root,piv_index);
		} else {
			adjust_with_left_sibling(root, piv_index);
		}
	} else {
		if (root->d.pivotmap[piv_index]->n+1+root->d.pivotmap[piv_index-1]->n >= MAX_SIZE) {
			adjust_with_left_sibling(root, piv_index);
		} else if (root->d.pivotmap[piv_index]->n+1+root->d.pivotmap[piv_index+1]->n >= MAX_SIZE) {
			adjust_with_right_sibling(root, piv_index);
		} else {
			merge_internal_nodes( root, piv_index);
			//merge_nodes(root,piv_index);
		}
	}

}

/**
* merge_data_nodes() will merge two internal nodes
* 
* Algorithm : 1.assign left_child, right_child => if piv_index !=0(i.e. child node is not the first child of parent and left sibling for the child is present)
*                                                        left_child = pivotmap[piv_index-1],  right_child = pivotmap[piv_index]
*                                                   else left_child = pivotmap[piv_index],  right_child = pivotmap[piv_index+1]
*             2. Merge all elements (key, value) from right_child  to left_child
*                                                    
* @param root is pointer to parent node of children which are going to be merged
* @param piv_index is index to child which need to get mergerd with one of the sibling
*/
void merge_data_nodes(node* root,int piv_index)
{
	/* leftchild is one in which we are going to merge content from right child */
	node* left_child = NULL, *right_child = NULL;
	int j;/*index from which pivot shifting is needed*/

	/*if left sibling is present */
	if (piv_index != 0) {
		/*merge with left sibling*/ 
		left_child = root->d.pivotmap[piv_index-1];
		right_child = root->d.pivotmap[piv_index];

		j = piv_index;
	} else {
		/*merge with right sibling */
		left_child = root->d.pivotmap[piv_index];
		right_child = root->d.pivotmap[piv_index+1];
		j = piv_index+1;
	}

	int left_child_n = left_child ->n;
	int right_child_n = right_child->n;

	for (int k = 0; k < right_child_n; k++) {
		left_child->key[left_child_n] =right_child->key[k];
		left_child->d.value[left_child_n] =right_child->d.value[k] ;
		left_child_n++;
	}
	left_child->n = left_child_n;
	/*shift pivot pointers of root to left as one of the child got merged with one of the siblings*/
	//temp_to_be_deleted = root->d.pivotmap[j]
	for(; j < root->n; j++) {
		root->key[j-1] = root->key[j];
		root->d.pivotmap[j] = root->d.pivotmap[j+1];
	}
	//delete /null the last child of root
	root->d.pivotmap[j] = NULL;

	/* update nextsib pointer*/
	left_child->nextsib = right_child->nextsib;
	root->n--;


	root->d.pivotmap[j] = NULL;
	//delete temp_to_be_deleted ;
	//temp_to_be_deleted = NULL;
	delete  right_child;
	right_child = NULL;

}


/**
* adjust_sibling() will get called when underflow occurs at node and one element can be borrowed from one of the sibling to become balance
* Algorithm : 1.adjust with left sibling : right shift element from current child by 1 index , 
*               add last element from left sibling at first of current child
*             OR 2.adjust with right sibling: add first element from right sibling to the last of current child,
*                  left shift element from right sibling by 1 index
* 
* @param root is pointer to parent node of child which need to be balanced
* @param sibling_flag is a flag for selecting sibling, if it's value is 1=>select left sibling else select right sibling 
* @param piv_index is index to child which need to get balanced with the help of one of the sibling
*/
void adjust_sibling(node* root, int sibling_flag,int piv_index)
{
	if (sibling_flag == 1) {
		/* select left sibling to make adjustment*/
		int left_sibling_n = root->d.pivotmap[piv_index-1]->n;
		int curr_child_n = root->d.pivotmap[ piv_index]->n;
		/*right shifting*/
		for (int k = curr_child_n; k > 0; k--) {
			root->d.pivotmap[ piv_index]->key[k] = root->d.pivotmap[ piv_index]->key[k-1];
			root->d.pivotmap[ piv_index]->d.value[k] = root->d.pivotmap[ piv_index]->d.value[k-1];
		}
		/* add max frm left sibling to current child */
		root->d.pivotmap[ piv_index]->key[0] = root->d.pivotmap[ piv_index-1]->key[left_sibling_n-1];
		root->d.pivotmap[ piv_index]->d.value[0] = root->d.pivotmap[ piv_index-1]->d.value[left_sibling_n-1];
		root->d.pivotmap[ piv_index]->n++;
		root->d.pivotmap[ piv_index-1]->n--;

	} else {
		/* select right sibling to make adjustment */
		int curr_child_n = root->d.pivotmap[ piv_index]->n;
		int right_sibling_n = root->d.pivotmap[piv_index+1]->n;
		/*add min frm right sibling to last of cutrrent child*/
		root->d.pivotmap[ piv_index]->key[curr_child_n] = root->d.pivotmap[ piv_index+1]->key[0];
		root->d.pivotmap[ piv_index]->d.value[curr_child_n] = root->d.pivotmap[ piv_index+1]->d.value[0];
		root->d.pivotmap[ piv_index]->n++;
		/*left shifting*/
		for (int k = 1; k < right_sibling_n; k++) {
			root->d.pivotmap[ piv_index+1]->key[k-1]= root->d.pivotmap[ piv_index+1]->key[k];
			root->d.pivotmap[ piv_index+1]->d.value[k-1]=root->d.pivotmap[ piv_index+1]->d.value[k];
		}
		root->d.pivotmap[ piv_index+1]->n--;
		/*update pivot index*/
		root->key[piv_index]= root->d.pivotmap[ piv_index+1]->key[0];
	}
}


/**
* balance_data_node() function will get call when underflow occur at data node
* This function will balance the node by making adjustment (i.e borrowing one elements from sibling) or merging with the sibling
* Priority is given to adjustment with the sibling than merging as  merge can be costly than adjusting
* Condition for merging OR adjusment: if sibling dosen't undergo underflow after removal of one element(key,value), adjustment can be done with that sibling
*                                     else merging need to be perform with any of the siblings
* Note: if both siblings are eligible for making adjustment with current child, sibling with greater number of keys gets priority 
*
* Algorithm : 1. set sibling_flag: set sibling_flag by considering weather left or right sibling exists or not 
*                and it is eligible for adjustment or not
*             2. if sibling_flag == 1 or 2 => call function adjust_sibling() to make node balance by adjusting with sibling
*                else, sibling_flag ==0, call function merge_data_nodes() to make node balance by merging with sibling
*
* @param root is pointer to parent node of child which need to be balanced
* @param piv_index is index to child which need to get balanced with the help of one of the sibling
*/
void balance_data_node(node* root,int piv_index)
{
	int minchild = MAX_SIZE%2? MAX_SIZE/2+1 : MAX_SIZE/2;
	int sibling_flag = 0; /*if sibling ==0, don't select any sibling  but merge sibling==1, select left sibling   if sibling ==2 ,select rightsibling*/
	if (piv_index == 0) {
		if (root->d.pivotmap[piv_index+1]->n > minchild-1) {
			sibling_flag = 2;
		}

	}else if(piv_index == root->n) {
		if ( root->d.pivotmap[piv_index-1]->n > minchild-1) {
			sibling_flag = 1;
		}
	} else {
		int left_sibling_n = root->d.pivotmap[piv_index-1]->n;
		int righ_sibling_n = root->d.pivotmap[piv_index+1]->n;
		if (left_sibling_n > righ_sibling_n) {  
			if (left_sibling_n > minchild-1) {
				sibling_flag  = 1;
			}
		} else {
			if (righ_sibling_n>minchild-1) {
				sibling_flag  =2;
			}
		}
	}
	if (sibling_flag) {
		adjust_sibling(root,sibling_flag,piv_index);
	}
	else {
		//cout<<"merge";
		//find sibling-child for merging 
		merge_data_nodes(root,piv_index);
		//merge_nodes(root,piv_index);
	}
}

/**
* delete_key() will delete key if present and take care if underflow occures at data node or internal node due to the deletion of key
* Algorithm : if node is data node => find the key and delete it if present
*             else find appropriate pivot for that key and call delete_key() recursively
*             
*             if underflow occur at data node => balance_data_node() function gets called to balance the node
*             if underflow occur at internal node => balance_internal_node() function gets called to balance the node
*
* @param root is pointer node from which key, value needs to be deleted
* @param key is a key which needs to get deleted
*/
int delete_key(node* root, int key) //delete key 
{
	int min_key  = -1;
	if (root->datanode) {
		for (int i = 0; i < root->n; i++) {
			if (root->key[i] == key) {
				for (int j = i; j < root->n-1; j++) {
					root->key[j] = root->key[j+1];
					root->d.value[j] = root->d.value[j+1]; 
				}
				root->n--;

				//cout<<"Given key, "<<key<<" is successfullly deleted..."<<endl;
				logFile<<"Given key, "<<key<<" is successfully deleted"<<endl;
				return  min_key;
			}
		}
		//cout<<"Given key, "<<key<<" is not present"<<endl;
		//logFile<<"Given key, "<<key<<" is not present"<<endl;
		errorlogFile<<"Given key, "<<key<<" is not present"<<endl;
		return  min_key;

	} else {
		/* find appropriate pivot pointer */
		int piv_index;
		for (piv_index = 0; piv_index < root->n; piv_index++) {
			if (key < root->key[piv_index]) {
				break;
			}
		}
		min_key = delete_key(root->d.pivotmap[piv_index], key);

		int minchild = MAX_SIZE%2? MAX_SIZE/2+1 : MAX_SIZE/2;

		if (root->d.pivotmap[piv_index]->datanode) {
			if(root->d.pivotmap[piv_index]->n < minchild - 1) {
				/* if underflow occure at data node */
				balance_data_node(root, piv_index);
			}
			if (piv_index != 0) {
				//root->key[piv_index-1] = root->d.pivotmap[piv_index]->key[0];
				if(piv_index<=root->n) {
					//condition root->d.pivotmap[piv_index] or piv_index<=root->n
					root->key[piv_index-1] = root->d.pivotmap[piv_index]->key[0];
				}	
			} else {
				//i.e. if i==0 and key deletion from first child//find deleteted key was min if yes =>update min_key  NOte: updating min_key needs to be done after bakancing
				if (key < root->d.pivotmap[0]->key[0]) {
					//condition becomes key <= root->d.pivotmap[0]->key[0] if dublicates allowed
					min_key = root->d.pivotmap[0]->key[0];
				}
			}
		} else { //if (!root->d.pivotmap[piv_index]->datanode) {
			if (piv_index != 0) {//updating minkey if applicable : note: updation needs to be done before balanncing

				if (root->key[piv_index-1] == key) {//i.e. minkey is deleted which is also present in internal node 
					//update that key
					root->key[piv_index-1]  = min_key;
					//2nd solution -> independent of return value i.e if delete_key return type void -> we need to traverse all the tree to find new minkey for that node-> but tie complexity will increase
				}
			}
			/* underflow occured at internal node */
			if(root->d.pivotmap[piv_index]->n < minchild - 1 ) {//root->d.pivotmap[i]->n<MAX_SIZE/2 

				balance_internal_node(root, piv_index);
			}
		}
	}
	return min_key;
}

/**
* delete_node() will call delete_key() function and delete root if required 
*
* @param root is pointer  to node pointing to root of B plus tree
* @param key is a key which needs to get deleted
*/
node* delete_node(node* root, int key)
{
	if(root != NULL) {
		delete_key(root,key);

		if (root->n == 0) {
			if (root->datanode == 0) {
				node* temp = root;
				root = root->d.pivotmap[0];
				delete temp;
				temp = NULL;
			} else {
				root = NULL;
			}
		}
		
	} else {
		cout<<"Tree is empty"<<endl<<endl;
	}
	return root;
}

/**
* search() will allow to search for a key and get value of that key if present
* Algorithm : if node is data node, traverse the node to get value of key if key is present 
*             else find appropriate pivot and call search() function recursively for that pivot i.e. search(P[i]) where i, such that key< key[i]
*
* @param root is pointer node at which key needs to get searched
* @param key is a key which needs to get searched
*/
void search(node* root, int key) //search for a key and return value
{
	if (root->datanode) {
		for (int i = 0; i < root->n; i++) {
			if (root->key[i] == key) {
				//cout<<"Value for given key,"<<key<<" is : "<<root->d.value[i]<<endl;
				logFile<<"Value for given key,"<<key<<" is : "<<root->d.value[i]<<endl;
				return ;
			}
		}
		//cout<<"Given key, "<<key<<" is not present"<<endl;
		//logFile<<"Given key, "<<key<<" is not present"<<endl;
		errorlogFile<<"Given key, "<<key<<" is not present"<<endl;
		return ;
	} else {
		/*find appropriate pivot pointer*/
		int i;
		for (i = 0; i < root->n; i++) {
			if (key < root->key[i]) {
				break;
			}
		}
		search(root->d.pivotmap[i], key);

	}
}

/**
* search_all() will provide all present key-value pairpresent in tree
* Algorithm : traverse to the leftiest data node and read the key-value present in that node and 
*             go to nextsibling using nextsib pointer present at node and read() ...repeat steps until nextsib!=NULL
*
* @param root is pointer to the root of tree
*/
void search_all(node* root) 
{
	if (root == NULL) {
		cout<<"search_all: Tree is Empty"<<endl;
		//logFile<<"search_all: Tree is Empty"<<endl;
		return;
	}

	/*traverse to leftist leftest data/leaf node */
	node *p = root;
	while (!p->datanode) {
		p=p->d.pivotmap[0];
	}
	while (p) {
		for (int i=0; i < p->n; i++) {
			cout<<"key : "<<p->key[i]<<"  value : "<<p->d.value[i]<<endl;
		}
		cout<<endl;
		p = p->nextsib;/* go to nextsibling*/
	}
}

/**
* get_interval_values() will provide all key(ki)-value pairs present in a given interval i.e. such that start_key<=Ki<=end_key
* Algorithm : search for start_key and reach the data node ,read the key-value present in that node such that start_key<=root->key[i] <= end_key
*             go to nextsibling using nextsib pointer present at node and read key, values such that key<=end_key
*             repeat steps until nextsib!=NULL or key>end_key
*
* @param root is pointer to the root of tree
* @param start_key is starting key of interval
* @param end_key is ending key of interval
*/
void get_interval_values(node* root, int start_key, int end_key) 
{
	if (root->datanode) {
		int i;
		for (i = 0; i < root->n; i++) {
			if (root->key[i] >= start_key) {
				break;
			}
		}

		int flag = 1,count = 0;
		do {
			for (; i < root->n; i++)
			{
				if (root->key[i] > end_key) {
					flag = 0;
					break;
				}
				cout<<"key : "<<root->key[i]<<" Value : "<<root->d.value[i]<<endl;
				count++;
			}
			root = root->nextsib;
			i = 0;
		} while (root != NULL && flag);
		cout<<"Total number of key -value pairs found in given interval : "<<count;

	} else {
		/*find appropriate pivot pointer*/
		int i;
		for (i = 0; i < root->n; i++) {
			if (start_key < root->key[i]) {
				break;
			}
		}
		get_interval_values(root->d.pivotmap[i], start_key, end_key);
	}
}

/**
* check_key_order() will check whether keys present in root are in increasing order or not
*
* @param root is node pointer where keys order needs to be checked
*/
bool check_key_order(node* root)
{
	for (int i = root->n-1; i > 0; i--) {
		if(root->key[i-1] > root->key[i]) {
			return 0;
		}
	}
	return 1;
}

/**
* invarient_b_plus_tree_() is function for invarient checking in depth first order
* Invarient checking:
* 1. keys should be in ascending order
* 2. the number of keys should be less than MAX_SIZE(i.e. order of tree)
* 3. the number of keys should be greater than minimum key required
* 4. Tree should be balance 
*
* return height of tree(to determine if tree is balance) if no violation else return -1
* @param root is node pointer for which invarient checking needs to be done
*/
int invarient_b_plus_tree_(node* root)
{
	int minchild = MAX_SIZE%2? MAX_SIZE/2+1 : MAX_SIZE/2;
	if (root->datanode) {
		//check keys in order
		if (!check_key_order(root)) {
			return -1;
		}
		//check  min,max n
		if (root->n > MAX_SIZE-1) {
			cout<<"Overflow at datanode: number of keys exceeds than value of maximum key required"<<endl;
			//logFile<<"Overflow at datanode: number of keys exceeds than value of maximum key required"<<endl;
			errorlogFile<<"Overflow at datanode: number of keys exceeds than value of maximum key required"<<endl;
			return -1;
		}
		if (root->n < minchild-1) {
			cout<<"Underflow at datanode: number of keys less than value minimum key required"<<endl;
			//logFile<<"Underflow at datanode: number of keys less than value minimum key required"<<endl;
			errorlogFile<<"Underflow at datanode: number of keys less than value minimum key required"<<endl;
			return -1;
		}
		return 0;
	}
	else {
		//check keys in order
		//check  min,max n
		if (root->n > MAX_SIZE-1) {
			cout<<"Overflow at internal node: number of keys exceeds than value of maximum key required"<<endl;
			//logFile<<"Overflow at internal node: number of keys exceeds than value of maximum key required"<<endl;
			errorlogFile<<"Overflow at internal node: number of keys exceeds than value of maximum key required"<<endl;

			return -1;
		}
		if (root->n < minchild-1) {
			cout<<"Underflow at internal node: number of keys less than value minimum key required"<<endl;
			//logFile<<"Underflow at internal node: number of keys less than value minimum key required"<<endl;
			errorlogFile<<"Underflow at internal node: number of keys less than value minimum key required"<<endl;

			return -1;
		}
		//traverse 
		int k  = invarient_b_plus_tree_(root->d.pivotmap[0]);
		if (k != -1) {
			for (int i=1; i <= root->n; i++) {
				int t = invarient_b_plus_tree_(root->d.pivotmap[i]);
				if(t != k) {
					cout<<" Tree is not balanced ...";

					//logFile<<" Tree is not balanced ...";
					errorlogFile<<" Tree is not balanced ...";
					return -1;
				}
			}
			return 1+k;
		} 

		return -1;

	}
}

/**
* check_invarient performs prerequisites for invarient_b_plus_tree_()
*
* return 0 if no violance  and 1, if some violence of rules in given B+tree
* @param root is node pointer pointing to root of B-plus tree
*/
bool check_invarient(node* root)
{
	if(root == NULL) {
		cout<<"INVARIENT : Tree is empty";
		//logFile<<"INVARIENT:Tree is empty";


	} else {
		//check invarient for root npde
		if (!check_key_order(root)) {
			return 0;
		}
		if (root->n > MAX_SIZE-1) {
			cout<<"Overflow at root: number of keys exceeds than value of maximum key required"<<endl;
			//logFile<<"Overflow at root: number of keys exceeds than value of maximum key required"<<endl;
			errorlogFile<<"Overflow at root: number of keys exceeds than value of maximum key required"<<endl;
			return 0;
		}
		if (root->n < 1) {
			cout<<"Underflow at root: number of keys less than value minimum key required"<<endl;
			//logFile<<"Underflow at root: number of keys less than value minimum key required"<<endl;
			errorlogFile<<"Underflow at root: number of keys less than value minimum key required"<<endl;

			return 0;
		}
		//check invarient for root internal children 
		if (!root->datanode) {
			int k  = invarient_b_plus_tree_(root->d.pivotmap[0]);
			if(k != -1) {

				for (int i=1; i<=root->n ;i++) {
					int t = invarient_b_plus_tree_(root->d.pivotmap[i]);

					if(t != k) {
						cout<<" Tree is not balanced ...";
						//logFile<<" Tree is not balanced ...";

						errorlogFile<<" Tree is not balanced ...";
						return  0;
					}
				}



			} 

		}
	}
	//cout<<"Invarient check finish withiout violation"<<endl;
	//logFile<<"Invarient check finish withiout violation"<<endl;

	return 1;
}
