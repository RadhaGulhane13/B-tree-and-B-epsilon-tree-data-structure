#include "B+ Tree.h"

void display(node* root) //function for displaying key ,pivots ,values in breadth first order
{
	queue<node*> g;
	g.push(root);
	while (!g.empty()) {
		node *n = g.front();
		g.pop();
		cout<<"datanode : "<<n->datanode<<"  total keys :"<<n->n<<" keys: ";
		for(int i=0;i<n->n;i++)
		{
			cout<<n->key[i]<<"  ";
		}
		if(n->datanode)
		{
			cout<<" values : ";
			for(int i=0;i<n->n;i++)
			{
				cout<<n->d.value[i]<<"  ";
			}

		}
		else
		{
			cout<<"PIVOTS :";
			if(!n->datanode){
				for (int i=0;i<n->n+1;i++) { 
					//cout<<n->pivotmap[i]->key[0]<<"  ";
					g.push(n->d.pivotmap[i]);
				}}

		}
		cout<<endl;

	}
	cout <<endl;


}

int split_internalnode_(node* root, int key, int index_) //if overflow occur at internal node, this function gets called and merge then split two nodes and return root key
{
	int half_maxsize = (MAX_SIZE-1)%2? (MAX_SIZE-1)/2 +1 : (MAX_SIZE-1)/2;

	node* n2 = new node;
	n2->datanode =0;
	n2->nextsib = NULL;

	int pivcount_n, ncount_n,pivcount_r, ncount_r,midkey;

	if(half_maxsize == index_) // if pivot which needs to be inserted at middle and midkey = temp
	{
		//cout<<endl<<endl<<"half_maxsize == index_"<<endl;
		midkey = key;
		n2->d.pivotmap[0] = root->d.pivotmap[index_]->nextsib;
		ncount_n=0;
		pivcount_n = 1;
		ncount_r = index_ ; // or half_maxsize
		pivcount_r = index_+1;
	}
	else if(index_< half_maxsize) //i.e. if pivot inserted in left sibling => need to make right shift in root as new element is going to be inserted at left sib
	{
		int temp_key;
		node* temp_node=NULL;
		int key_ = key;
		node* node_ = root->d.pivotmap[index_]->nextsib;
		int i;
		for(i=index_;i<half_maxsize;i++)
		{
			temp_key= root->key[i];
			temp_node = root->d.pivotmap[i+1];

			root->key[i] = key_;
			root->d.pivotmap[i+1] = node_; 

			key_ = temp_key;
			node_ = temp_node;

		}

		midkey = key_;
		n2->d.pivotmap[0] = node_;
		ncount_n=0;
		pivcount_n = 1;
		ncount_r =  i; // or half_maxsize
		pivcount_r = i+1;

	}
	else
	{
		midkey = root->key[half_maxsize];
		ncount_n=0;
		pivcount_n=0;
		for(int i=half_maxsize+1;i<index_;i++)
		{
			n2->key[ncount_n++] = root->key[i];
			n2->d.pivotmap[pivcount_n++] = root->d.pivotmap[i];
		}
		n2->key[ncount_n++] = key;
		n2->d.pivotmap[pivcount_n++] = root->d.pivotmap[index_];
		n2->d.pivotmap[pivcount_n++] = root->d.pivotmap[index_]->nextsib;

		ncount_r = index_;
		pivcount_r = index_+1;



	}

	for(int j = ncount_r;j<root->n;j++)
	{
		n2->key[ncount_n] = root->key[j];
		n2->d.pivotmap[ncount_n+1] = root->d.pivotmap[j+1];
		ncount_n++;
	}


	//update value of n for root and n2;
	root->n = half_maxsize;
	n2->n = ncount_n;

	root->nextsib = n2;


	return midkey;
}

int split_(node* root, int key,int value)  //when overflow occure at data node ,this function gets called  : it will split node and return middle key
{
	int half_maxsize = MAX_SIZE%2? MAX_SIZE/2 +1 : MAX_SIZE/2;

	node* n2 = new node;
	n2->datanode =1;

	int i,flag = 0, count_n, count_r;
	for(i=0;i<half_maxsize;i++)
	{
		//if(key<root->key[i]){//key already present; return -1;}else
		if(key<root->key[i])
		{
			int temp_key,temp_value;
			for(;i<half_maxsize;i++)
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


	if(flag)
	{
		n2->key[0] = key;
		n2->d.value[0] = value;
		count_n = 1;
		count_r = i;
	}
	else
	{
		count_n = 0;
		for(;i<root->n;i++)
		{
			if(key < root->key[i])
			{
				n2->key[count_n] = key;
				n2->d.value[count_n] = value;
				count_n ++;

				count_r = i;
				break;

			}
			else
			{
				n2->key[count_n] = root->key[i];
				n2->d.value[count_n] = root->d.value[i];
				count_n++;
			}
		}

		if(i == root->n) // when new elemet needs to be inserted at end
		{
			//cout<<"i == root->n";
			n2->key[count_n] = key;
			n2->d.value[count_n] = value;
			count_n++;
		}

	}
	for(;i<root->n;i++)
	{ 
		//cout<<"i != root->n   ";
		n2->key[count_n] = root->key[i];
		n2->d.value[count_n] = root->d.value[i];
		count_n++;
	}

	root->n = half_maxsize ;
	n2->n = count_n;

	n2->nextsib = root->nextsib;
	root->nextsib = n2;
	//cout<<endl<<endl;
	return n2->key[0];

}

int insert(node* root, int key,int value) //this function will insert key to appropriate location and take care of splitting datanode or internal node
{
	/*if(root == NULL) //when tree is empty
	{
	root= new node;
	root->datanode =1;
	root->key[0] = key;
	root->d.value[0]=value;
	root->n =1;
	root->nextsib = NULL;
	}
	else
	{*/
	if(!root->datanode)//i.e. node is internal-indexed node
	{
		//find appropriate pivot
		int i;
		for(i=0;i<root->n;i++)
		{
			if(key<root->key[i])
			{
				break;
			}
		}
		int prev = root->d.pivotmap[i]->n;
		bool prev_datanode = root->d.pivotmap[i]->datanode;
		int temp = insert(root->d.pivotmap[i], key, value);

		if( temp != -1)
		{
			if(root->n==MAX_SIZE-1)//i.e. pivotmpa is full =>split again
			{
				//split again;
				//passing i to function to avoid traversal to find where key(temp) fits
				return split_internalnode_(root, temp, i);
			}
			else
			{//add temp   to current root

				//add indexes
				for(int j = root->n;j>i;j--)
				{
					root->key[j]=root->key[j-1];
				}
				root->key[i]=temp;

				//add pivots
				for(int j = root->n+1;j>i+1;j--)
				{
					root->d.pivotmap[j]=root->d.pivotmap[j-1];
				}
				root->d.pivotmap[i+1]=root->d.pivotmap[i]->nextsib;

				root->n++;

				return -1;
			}
		}
		return -1;


	}
	else//i.e. node is data node/leaf node
	{
		if(root->n == MAX_SIZE-1)//if datanode is full =>split MAX_SIZE-1
		{
			//cout<<"datanode is full :split";

			return split_(root,key,value);
		}
		else
		{
			//add key to appropriate location

			int j;
			for(j=0;j<root->n;j++)
			{
				if(key<root->key[j])
				{
					break;
				}
			}
			for(int k = root->n;k>j;k--)
			{
				root->key[k]=root->key[k-1];
				root->d.value[k]=root->d.value[k-1];
			}
			root->key[j]= key;
			root->d.value[j]= value;
			root->n++;


		}
	}
	//}
	return -1;

}

node* insert_node(node* root, int key,int value) //insert_node() create new root node if required
{
	if(root==NULL)
	{
		root= new node;
		root->datanode =1;
		root->key[0] = key;
		root->d.value[0]=value;
		root->n =1;
		root->nextsib = NULL;

	}else
	{
		int result = insert(root,key,value);
		//cout<<"result: "<<result<<"  ";
		if(result !=-1)
		{
			node *new_node = new node;
			new_node->key[0] = result;
			new_node->d.pivotmap[0] = root;
			new_node->d.pivotmap[1] = root->nextsib;
			new_node->n = 1;
			new_node->datanode =0;

			new_node->nextsib=NULL;

			root = new_node;
		}
	}
	return root;
}

void delete_key(node* root, int key) //delete key 
{
	if(root->datanode == 1 )
	{
		for(int i=0;i<root->n;i++)//for now:linear search is used but any searching data-strctyue like :binary search can be implementend to search key in data node 
		{
			if(root->key[i] == key)
			{
				//delete the key-value
				for(int j=i;j<root->n-1;j++)
				{
					root->key[j]=root->key[j+1];
					root->d.value[j]=root->d.value[j+1]; 
				}
				root->n--;

				cout<<"Given key, "<<key<<" is successfullly deleted..."<<endl;
				return;
			}
		}
		cout<<"Given key, "<<key<<" is not present"<<endl;
		return;

	}
	else //find appropriate pivot pointer
	{
		int i;
		for(i=0;i<root->n;i++)
		{
			if(key<root->key[i])
			{
				break;
			}
		}

		delete_key(root->d.pivotmap[i], key);
		//update pivot-key
		/*if(i!=0){
		root->key[i-1] = root->d.pivotmap[i]->key[0];
		}*/
		if(root->d.pivotmap[i]->datanode == 1)//if pivot is datanode  root->d.pivotmap[i]->datanode == 1&&root->d.pivotmap[i]->n<MAX_SIZE/2
		{

			if(root->d.pivotmap[i]->n<MAX_SIZE/2)//if underflow at datanode
			{
				int left_sibling_n = -1;
				int righ_sibling_n = -1;
				//1.check sibling to adjust :select sibling with maximum 'n' for adjustment if any
				//2. else merge

				int sibling_flag = 0; //if sibling =0... don't select any sibling  but merge sibling=1 ..select left sibling   if sibling = 2 ...select rightsibling
				if(i == 0)
				{
					righ_sibling_n = root->d.pivotmap[i+1]->n;

					if(righ_sibling_n > MAX_SIZE/2)
					{
						sibling_flag = 2;
					}

				}else if(i == root->n)
				{
					left_sibling_n = root->d.pivotmap[i-1]->n;
					if( left_sibling_n > MAX_SIZE/2)
					{
						sibling_flag = 1;
					}
				}else
				{
					left_sibling_n = root->d.pivotmap[i-1]->n;
					righ_sibling_n = root->d.pivotmap[i+1]->n;
					if(left_sibling_n > righ_sibling_n)
					{ 
						if(left_sibling_n>MAX_SIZE/2)
						{
							sibling_flag  = 1;
						}
					}
					else
					{
						if(righ_sibling_n>MAX_SIZE/2)
						{
							sibling_flag  =2;
						}
					}
				}

				if(sibling_flag == 1) //i.e. select left sibling to make adjustment
				{
					for(int k =0;i<root->d.pivotmap[i]->n;k++)
					{
						root->d.pivotmap[i]->key[k+1]= root->d.pivotmap[i]->key[k];
						root->d.pivotmap[i]->d.value[k+1]=root->d.pivotmap[i]->d.value[k];
					}
					//add max frm left sibling to current child
					root->d.pivotmap[i]->key[0]=root->d.pivotmap[i-1]->key[left_sibling_n-1];
					root->d.pivotmap[i]->d.value[0]=root->d.pivotmap[i-1]->d.value[left_sibling_n-1];
					root->d.pivotmap[i]->n++;
					root->d.pivotmap[i-1]->n--;
					//update pivot index
					//root->key[i-1]= root->d.pivotmap[i]->key[0];

				}else if( sibling_flag == 2) //i.e. select right sibling to make adjustment
				{

					int currchild_n= root->d.pivotmap[i]->n;
					//add min frm right sibling to last of cutrrent child
					root->d.pivotmap[i]->key[currchild_n]=root->d.pivotmap[i+1]->key[0];
					root->d.pivotmap[i]->d.value[currchild_n]=root->d.pivotmap[i+1]->d.value[0];
					root->d.pivotmap[i]->n++;


					for(int k =1;k<righ_sibling_n;k++)
					{
						root->d.pivotmap[i+1]->key[k-1]= root->d.pivotmap[i+1]->key[k];
						root->d.pivotmap[i+1]->d.value[k-1]=root->d.pivotmap[i+1]->d.value[k];
					}
					root->d.pivotmap[i+1]->n--;
					//update pivot index
					root->key[i]= root->d.pivotmap[i+1]->key[0];

				}else //i.e. merg
				{
					//find sibling-child for merging 
					if(i!=0) //if left sibling is present 
					{
						//int sibling_n =root->d.pivotmap[i-1]->n;
						//merge with left sibling 
						for(int k=0;k<root->d.pivotmap[i]->n;k++)
						{
							root->d.pivotmap[i-1]->key[left_sibling_n] =root->d.pivotmap[i]->key[k];
							root->d.pivotmap[i-1]->d.value[left_sibling_n] =root->d.pivotmap[i]->d.value[k] ;
							left_sibling_n++;


						}
						root->d.pivotmap[i-1]->n = left_sibling_n;

						for(int k = i;k<root->n;k++)
						{
							root->key[k-1]=root->key[k];
							root->d.pivotmap[k]=root->d.pivotmap[k+1];
						}
						root->n--;
						//delete /null the last child of root

						//todo:find if overflow can occur if yes ->handel
					}
					else
					{
						//int sibling_n =root->d.pivotmap[i+1]->n;
						//merge with right sibling 
						int currchild_n =root->d.pivotmap[i]->n;
						for(int k=0;k<righ_sibling_n;k++)
						{
							root->d.pivotmap[i]->key[currchild_n] = root->d.pivotmap[i+1]->key[k];
							root->d.pivotmap[i]->d.value[currchild_n] = root->d.pivotmap[i+1]->d.value[k];
							currchild_n++;

						}
						root->d.pivotmap[i]->n = currchild_n;

						for(int k=i;k<root->n-1;k++)
						{
							root->key[k] = root->key[k+1];
							root->d.pivotmap[k+1] = root->d.pivotmap[k+2];
						}
						root->n--;

						//delete /null the last child of root

						//todo:find if overflow can occur if yes ->handel
					}

				}



			}
			if(i!=0)
			{
				root->key[i-1] = root->d.pivotmap[i]->key[0];
			}



		}

		if(root->d.pivotmap[i]->datanode == 0 && root->d.pivotmap[i]->n<MAX_SIZE/2) //underflow occured at internal node
		{
			//cout<<"underflow occured at internal node";
			//find appropriate sibling to merge :choose sibling with min n so that less chances of overflow
			if(i!=0) //merge with left 
			{

				if(root->d.pivotmap[i]->n+1+root->d.pivotmap[i-1]->n  <MAX_SIZE)
				{
					//merge with left delete right pivot
					int sib_n = root->d.pivotmap[i-1]->n ;
					root->d.pivotmap[i-1]->key[sib_n] =root->key[i-1];
					int k=0;
					for(;k<root->d.pivotmap[i]->n;k++)
					{
						root->d.pivotmap[i-1]->d.pivotmap[sib_n+1] = root->d.pivotmap[i]->d.pivotmap[k];
						sib_n++;
						root->d.pivotmap[i-1]->key[sib_n] = root->d.pivotmap[i]->key[k];

					}

					root->d.pivotmap[i-1]->d.pivotmap[sib_n+1] = root->d.pivotmap[i]->d.pivotmap[k];
					sib_n++;
					root->d.pivotmap[i-1]->n = sib_n;


					//adjust other sibling pointer: as p[i] will get deleted shift other sibling to left  and delete/null last child :
					for(int k = i;k<root->n;k++)
					{
						root->key[k-1]=root->key[k];
						root->d.pivotmap[k]=root->d.pivotmap[k+1];
					}
					root->n--;

				}
				else //overflow can occur : jsut move key-pivot frm left to right one by one and set root->key[i-1]=max of pivot i
				{
					int child_n = root->d.pivotmap[i]->n;
					int temp_midkey = root->key[i-1];
					int total = root->d.pivotmap[i-1]->n + child_n ;
					int remain = total - total /2 -child_n;
					int k;
					for(k=child_n-1;k>=0;k--)
					{
						root->d.pivotmap[i]->key[k+remain] = root->d.pivotmap[i]->key[k];
						root->d.pivotmap[i]->d.pivotmap[k+remain+1] = root->d.pivotmap[i]->d.pivotmap[k+1];

					}
					root->d.pivotmap[i]->d.pivotmap[k+remain+1] = root->d.pivotmap[i]->d.pivotmap[k+1];

					root->d.pivotmap[i]->n = child_n+remain;
					int sib_n = root->d.pivotmap[i-1]->n;


					for(int k_=remain;k_>0;k_--)
					{
						root->d.pivotmap[i]->key[k+k_]= temp_midkey;
						temp_midkey = root->d.pivotmap[i-1]->key[sib_n-1];
						root->d.pivotmap[i]->d.pivotmap[k+k_] = root->d.pivotmap[i-1]->d.pivotmap[sib_n];
						sib_n--;

					}
					root->key[i-1]= temp_midkey;
					root->d.pivotmap[i-1]->n = sib_n;
					//assert piv[i],piv[i-1] not overflowed, underflowed


				}
			}
			else //merge with right
			{
				if(root->d.pivotmap[i]->n+1+root->d.pivotmap[i+1]->n  <MAX_SIZE)
				{
					//merge with left delete right pivot
					int sib_n = root->d.pivotmap[i+1]->n;
					int child_n = root->d.pivotmap[i]->n;
					root->d.pivotmap[i]->key[child_n] =root->key[i];
					//root->d.pivotmap[i]->n++;

					int k=0;
					for(;k<sib_n;k++)
					{
						child_n++;
						root->d.pivotmap[i]->d.pivotmap[child_n] = root->d.pivotmap[i+1]->d.pivotmap[k];
						root->d.pivotmap[i]->key[child_n] = root->d.pivotmap[i+1]->key[k];

					}
					child_n++;
					root->d.pivotmap[i]->d.pivotmap[child_n] = root->d.pivotmap[i+1]->d.pivotmap[k];

					root->d.pivotmap[i]->n = child_n;


					//adjust other sibling pointer: as p[i] will get deleted shift other sibling to left  and delete/null last child :
					for(int k = i+1;k<root->n;k++)
					{
						root->key[k-1]=root->key[k];
						root->d.pivotmap[k]=root->d.pivotmap[k+1];
					}
					root->n--;

				}
				else  //overflow can occur
				{
					//cout<<"overflow when merge with right sibling";
					int child_n = root->d.pivotmap[i]->n;
					int temp_midkey = root->key[i];
					int total = child_n +root->d.pivotmap[i+1]->n ;
					int remain = total - total /2 -child_n;

					for(int k=0;k<remain;k++)
					{
						root->d.pivotmap[i]->key[child_n++]= temp_midkey;
						temp_midkey = root->d.pivotmap[i+1]->key[k];
						root->d.pivotmap[i]->d.pivotmap[child_n] = root->d.pivotmap[i+1]->d.pivotmap[k];

					}
					root->key[i]= temp_midkey;
					root->d.pivotmap[i]->n = child_n;
					//move element in right child by index couunt to right
					int k;
					for(k=remain;k<root->d.pivotmap[i+1]->n;k++)
					{
						root->d.pivotmap[i+1]->key[k-remain] = root->d.pivotmap[i+1]->key[k];
						root->d.pivotmap[i+1]->d.pivotmap[k-remain] = root->d.pivotmap[i+1]->d.pivotmap[k];

					}
					root->d.pivotmap[i+1]->d.pivotmap[k-remain] = root->d.pivotmap[i+1]->d.pivotmap[k];
					root->d.pivotmap[i+1]->n = root->d.pivotmap[i+1]->n-remain;
					//assert piv[i],piv[i+1] not overflowed, underflowed

				}


			}
		}


	}

}

void search(node* root,int key) //search for a key and return value
{

	if(root->datanode == 1 )
	{
		for(int i=0;i<root->n;i++)//for now:linear search but any search data-strctyue like :binary search can be implementend
		{
			if(root->key[i] == key)
			{
				cout<<"Value for given key,"<<key<<" is : "<<root->d.value[i]<<endl;
				return ;
			}
		}
		cout<<"Given key, "<<key<<" is not present"<<endl;
		return ;
	}
	else //find appropriate pivot pointer
	{
		int i;
		for(i=0;i<root->n;i++)
		{
			if(key<root->key[i])
			{
				break;
			}
		}
		search(root->d.pivotmap[i], key);

	}
}

void search_all(node* root) //search all key-values with time complexity = time complexity to go to leftest datanode + time complexity to traverse all next siblings(i.e.n-1 datanode)
{

	//traverse to leftist child(i.e. leftest data/leaf node )

	node *p= root;
	while(p->datanode == 0)
	{
		p=p->d.pivotmap[0];
	}
	while(p!=NULL)
	{
		for(int i=0;i<p->n;i++)
		{
			cout<<"key : "<<p->key[i]<<"  value : "<<p->d.value[i]<<endl;
		}
		cout<<endl;
		p=p->nextsib;
	}


}

void get_interval_values(node* root, int start_key, int end_key) //get key(ki) ,value such that start_key<=Ki<=end_key
{

	if(root->datanode == 1 )
	{
		int i ;
		for(i=0;i<root->n;i++)//for now:linear search but any search data-strctyue like :binary search can be implementend
		{
			if(root->key[i] >= start_key)
			{
				//cout<<"Value for key,"<<start_key<<" is : "<<p->d.value[i]<<endl;
				break;
			}
		}
		if(i == root->n)
		{
			cout<<"No key- value pair between such interval ... "<<endl;
		}
		else
		{
			int flag = 1;
			do{
				for(;i<root->n;i++)
				{

					if( root->key[i] > end_key )
					{ flag = 0;
					break;
					}
					cout<<"key : "<<root->key[i]<<" Value : "<<root->d.value[i]<<endl;

				}
				root = root->nextsib;
				i=0;
			}while(root!= NULL && flag );

		}
	}
	else //find appropriate pivot pointer
	{
		int i;
		for(i=0;i<root->n;i++)
		{
			if(start_key < root->key[i])
			{
				break;
			}
		}
		get_interval_values(root->d.pivotmap[i], start_key, end_key);

	}
}
