#include<iostream>
#include<map>
#include<queue>
using namespace std;

const int MAX_SIZE = 3;

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
};

void display(node* root)
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
	cout << '\n';


}

node* initialize(node* root)
{
	root= new node;
	for(int i=0;i<MAX_SIZE+1;i++)
	{

		root->d.pivotmap[i]=NULL;

	}
	root->datanode =1;
	return root;
}

node* split_internalnode(node* root,node* temp)
{
	node *n1 = NULL;
	n1 = initialize(n1);
	n1->datanode=0;

	node *n2 = NULL;
	n2 = initialize(n2);
	n2->datanode=0;

	int flag =0,rdatacount=0,rpivcount=0,datacount=0,pivcount=0,i;
	for(i=0;i<MAX_SIZE/2;i++)
	{
		if(flag == 0 && temp->key[0]<root->key[rdatacount])
		{
			flag=1;
			n1->key[datacount++]=temp->key[0];
			n1->d.pivotmap[pivcount++]=temp->d.pivotmap[0];
			if(pivcount== MAX_SIZE/2+1)
			{
				n2->d.pivotmap[0]=temp->d.pivotmap[1];

			}else
			{
				n1->d.pivotmap[pivcount++]=temp->d.pivotmap[1];
			}
			rpivcount++;
		}else
		{
			n1->key[datacount++]=root->key[rdatacount++];
			n1->d.pivotmap[pivcount++]=root->d.pivotmap[rpivcount++];
		}

	}

	int temp_;
	//int temp_ = root->key[rdatacount++];
	n1->n=datacount;
	datacount=0;
	if(flag == 0)//i.e. pivcount!=MAX_SIZE/2+1 (1 in case of 3)
	{
		if(temp->key[0]<root->key[rdatacount])
		{
			flag =1;
			temp_ = temp->key[0];
			n1->d.pivotmap[MAX_SIZE/2]=temp->d.pivotmap[0];
			n2->d.pivotmap[0]=temp->d.pivotmap[1];

			rpivcount++;

		}else
		{
			n1->d.pivotmap[pivcount++]=root->d.pivotmap[rpivcount++];
			temp_ = root->key[rdatacount++];
		}
	}else
	{
		temp_ = root->key[rdatacount++];
	}
	if(n2->d.pivotmap[0]==NULL)
	{
		pivcount=0;
	}
	else
	{
		pivcount=1;
	}


	for(int j=i;j<MAX_SIZE-1;j++)
	{
		if(rdatacount == root->n)//temp can be present at last 
		{
			flag=1;
			n2->key[datacount++]=temp->key[0];
			n2->d.pivotmap[pivcount++]=temp->d.pivotmap[0];

			n2->d.pivotmap[pivcount++]=temp->d.pivotmap[1];
			rpivcount++;
			break;
		}else
		{
			if(flag ==0 && temp->key[0]<root->key[rdatacount])
			{ 
				flag=1;
				n2->key[datacount++]=temp->key[0];
				n2->d.pivotmap[pivcount++]=temp->d.pivotmap[0];

				n2->d.pivotmap[pivcount++]=temp->d.pivotmap[1];
				rpivcount++;
			}else
			{//cout<<"rdatacount:"<<rdatacount<<" rpivcount"<<rpivcount<<" datacount"<<datacount<<" pivcount"<<pivcount;
				n2->key[datacount++]=root->key[rdatacount++];
				n2->d.pivotmap[pivcount++]=root->d.pivotmap[rpivcount++];
			}}
	}

	if(pivcount ==datacount)
	{
		n2->d.pivotmap[pivcount++]=root->d.pivotmap[rpivcount++];
	}
	n2->n=datacount;

	root->n = 1;
	root->key[0] = temp_;
	root->d.pivotmap[0] = n1;
	root->d.pivotmap[1] = n2;
	return root;
}

node* split(node* root, int key,int value)
{
	node *n1 = NULL;
	n1 = initialize(n1);


	int count=0,i;
	for(i =0;i<=MAX_SIZE/2;i++)
	{
		if(key<root->key[count])
		{
			n1->key[i]=key;
			n1->d.value[i]= value;
			key = root->key[count];
			value = root->d.value[count];
		}
		else
		{
			n1->key[i] = root->key[count];
			n1->d.value[i] = root->d.value[count];

		}
		count++;
	}
	n1->n = i;
	node* n2 = NULL;
	n2 = initialize(n2);
	n2->n = MAX_SIZE-1-n1->n+1;// MAX_SIZE-1-n1->n+1;
	for(int j =0;j<n2->n;j++)
	{
		if(count == root->n)
		{
			n2->key[j]=key;
			n2->d.value[j] = value;

			break;
		}
		if(key<root->key[count])
		{
			n2->key[j]=key;
			n2->d.value[j] = value;
			key = root->key[count];
			value = root->d.value[count];
		}
		else
		{
			n2->key[j] = root->key[count];
			n2->d.value[j] = root->d.value[count];

		}count++;
	}

	root->key[0] = n2->key[0];
	root->d.pivotmap[0] = n1;
	root->d.pivotmap[1] = n2;
	root->n=1;
	root->datanode =0;
	//cout<<"in split : ";
	//display(root);
	return root;

}

node* insert(node* root, int key,int value)
{
	if(root == NULL)
	{
		root= initialize(root);
		root->key[0] = key;
		root->d.value[0]=value;
		root->n =1;
	}
	else
	{
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
			node* temp = insert(root->d.pivotmap[i], key, value);
			//if(temp->n ==1) //add new  root to current if n->data<max else split

			int flag_=0;

			if(prev_datanode!=temp->datanode)
			{
				flag_ =1;
			}else 
			{

				if(temp->datanode == 0  && temp->n != prev && temp->n==1)//or temp<MAX/2//this condition :temp->n != prev is require as prev can be one, for MAX_VALUE=3 
				{
					flag_ =1;
				}
			}
			//cout<<"flag_ :"<<flag_<<endl;
			if( flag_ == 1)
			{
				if(root->n==MAX_SIZE-1)//i.e. pivotmpa is full =>split again
				{
					//cout<<"split again";

					return split_internalnode(root,temp);
				}
				else
				{//add temp   to current root
					//add indexes
					//cout<<"add indexes";
					for(int j = root->n;j>i;j--)
					{
						root->key[j]=root->key[j-1];
					}
					root->key[i]=temp->key[0];

					//add pivots
					for(int j = root->n+1;j>i+1;j--)
					{//cout<<"adding pivots";
						root->d.pivotmap[j]=root->d.pivotmap[j-1];
					}
					root->d.pivotmap[i]=temp->d.pivotmap[0];
					root->d.pivotmap[i+1]=temp->d.pivotmap[1];

					root->n++;
					//as above we are creating extra node pointer i.e. temp... so we can copy that to current and delete temp make temp null just to avoid use of memory or optimization of code 

					return root;
				}
			}
			//return root;


		}
		else//i.e. node is data node/leaf node
		{
			if(root->n ==MAX_SIZE-1)//if datanode is full =>split MAX_SIZE-1
			{
				//cout<<"datanode is full :split";
				node* result = split(root,key,value);
				return result;
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
	}
	return root;

}

node* delete_key(node* root, int key)
{
	if(root->datanode == 1 )
	{
		for(int i=0;i<root->n;i++)//for now:linear search but any search data-strctyue like :binary search can be implementend
		{
			if(root->key[i] == key)
			{
				//cout<<"Value for given key,"<<key<<" is : "<<root->d.value[i]<<endl;
				//delete the key-value
				for(int j=i;j<root->n-1;j++)
				{
					root->key[j]=root->key[j+1];
					root->d.value[j]=root->d.value[j+1]; 
				}
				root->n--;
				return root;
			}
		}
		cout<<"Given key, "<<key<<" is not present"<<endl;
		return root;
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
		root->d.pivotmap[i] = delete_key(root->d.pivotmap[i], key);

		//update pivot-key
		/*if(i!=0){
		root->key[i-1] = root->d.pivotmap[i]->key[0];
		}*/
		if(root->d.pivotmap[i]->datanode == 1)//if pivot is datanode  root->d.pivotmap[i]->datanode == 1&&root->d.pivotmap[i]->n<MAX_SIZE/2
		{

			if(root->d.pivotmap[i]->n<MAX_SIZE/2)//if underflow at datanode
			{
				//1.check sibling to adjust

				if(i!=0)//check left sibling
				{ 
					int sibling_n = root->d.pivotmap[i-1]->n;
					if(sibling_n>MAX_SIZE/2)//sib-1>=ceil(max/2)-1
					{
						for(int k =0;i<root->d.pivotmap[i]->n;k++)
						{
							root->d.pivotmap[i]->key[k+1]= root->d.pivotmap[i]->key[k];
							root->d.pivotmap[i]->d.value[k+1]=root->d.pivotmap[i]->d.value[k];
						}
						//add max frm left sibling to current child
						root->d.pivotmap[i]->key[0]=root->d.pivotmap[i-1]->key[sibling_n-1];
						root->d.pivotmap[i]->d.value[0]=root->d.pivotmap[i-1]->d.value[sibling_n-1];
						root->d.pivotmap[i]->n++;
						root->d.pivotmap[i-1]->n--;
						//update pivot index
						//root->key[i-1]= root->d.pivotmap[i]->key[0];

					}
				}
				if(root->d.pivotmap[i]->n < MAX_SIZE/2)
				{
					//cout<<" right sibling";
					if(i!=root->n)
					{
						int sibling_n = root->d.pivotmap[i+1]->n;
						if(sibling_n>MAX_SIZE/2)
						{
							int currchild_n= root->d.pivotmap[i]->n;
							//add min frm right sibling to last of cutrrent child
							root->d.pivotmap[i]->key[currchild_n]=root->d.pivotmap[i+1]->key[0];
							root->d.pivotmap[i]->d.value[currchild_n]=root->d.pivotmap[i+1]->d.value[0];
							root->d.pivotmap[i]->n++;


							for(int k =1;k<sibling_n;k++)
							{
								root->d.pivotmap[i+1]->key[k-1]= root->d.pivotmap[i+1]->key[k];
								root->d.pivotmap[i+1]->d.value[k-1]=root->d.pivotmap[i+1]->d.value[k];
							}
							root->d.pivotmap[i+1]->n--;
							//update pivot index
							root->key[i]= root->d.pivotmap[i+1]->key[0];
						}
					}
				}

				//else merge
				if(root->d.pivotmap[i]->n < MAX_SIZE/2)
				{
					//find sibling-child for merging 
					if(i!=0) //if left sibling is present 
					{
						int sibling_n =root->d.pivotmap[i-1]->n;
						//merge with left sibling 
						for(int k=0;k<root->d.pivotmap[i]->n;k++)
						{
							root->d.pivotmap[i-1]->key[sibling_n] =root->d.pivotmap[i]->key[k];
							root->d.pivotmap[i-1]->d.value[sibling_n] =root->d.pivotmap[i]->d.value[k] ;
							sibling_n++;


						}
						root->d.pivotmap[i-1]->n = sibling_n;

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
						int sibling_n =root->d.pivotmap[i+1]->n;
						//merge with right sibling 
						int currchild_n =root->d.pivotmap[i]->n;
						for(int k=0;k<sibling_n;k++)
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
			//find appropriate sibling to merge :chose sibling with min n so that less chances of overflow
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

					/*int temp_midkey = root->key[i-1];
					int total = root->d.pivotmap[i]->n+1+root->d.pivotmap[i-1]->n ;
					int first = total/2;
					int second = total - first- 1;
					int curr =root->d.pivotmap[i]->n;
					int remain = second - curr;
					int j;
					for(j=curr-1;j>=0;j--)
					{
						root->d.pivotmap[i]->key[j+remain]= root->d.pivotmap[i]->key[j];
						root->d.pivotmap[i]->d.pivotmap[j+remain+1]= root->d.pivotmap[i]->d.pivotmap[j+1];
					}
					root->d.pivotmap[i]->d.pivotmap[j+remain+1]= root->d.pivotmap[i]->d.pivotmap[j+1];

					remain--;
					int sib_n=root->d.pivotmap[i-1]->n; 
					while(remain>=0)
					{
						root->d.pivotmap[i]->key[j+remain] = temp_midkey;

						temp_midkey = root->d.pivotmap[i-1]->key[sib_n-1];
						root->d.pivotmap[i]->d.pivotmap[j+remain+1]= root->d.pivotmap[i-1]->d.pivotmap[sib_n];
						sib_n--;
						remain--;
					}
					root->d.pivotmap[i-1]->n=sib_n;
					root->key[i-1]= temp_midkey;*/

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

		return root;
	}

}


void search(node* root,int key)
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

int main()
{

	node *root=NULL;
	/*
	int n,key,value;
	cout<<"Enter number of element : "<<endl;
	cin>>n;
	for(int i=0;i<n;i++)
	{
	cout<<"Enter Key value : ";
	cin>>key;
	//cin>>value;
	root = insert(root,key,key+1);

	display(root);
	}*/

	//int arr[]={3,18,14,30,32,36,15,16,12,40,45,38,39,11,6,10};
	//int arr[]={3,18,14,30,32,36,15,16,12,40,45,38,39,11,6,10,46,47,48};
	int arr[]={3,18,14,30,32,36,15,16,12,40,45,38,39,11,6,10,31,35,37};
	for(int i=0;i<19;i++)
	{
		cout<<"INSERT : "<<arr[i]<<endl;
		root = insert(root,arr[i],arr[i]+1);

		display(root);
		cout<<endl;
	}
	int key_;
	for(int i=0;i<19;i++)
	{
		cout<<"Enter key for delete : ";
		cin>>key_;
		root=delete_key(root,key_);
		if(root->n == 0)
		{
			//node*temp = root;
			root = root->d.pivotmap[0];
			//temp=NULL;
			//delete temp;
		}
		display(root);
		cout<<endl;
	}
	/*for(int i=0;i<20;i++)
	{
	cout<<"Enter key for search : ";
	cin>>key_;
	search(root,key_);
	}*/

}

