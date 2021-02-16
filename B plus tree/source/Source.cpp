#include "B+ Tree.h"

int main()
{
	cout<<"B PLUS TREE V4"<<endl;
	node *root=NULL;

	//OPERATIONS :1.INSERT  2.DELETE  3.SEARCH  4.DISPLAY 5.SEARCH ALL KEY-VALUES FOR AN INTERVAL  6.GET ALL KEY-VALUES
	int q;

	do
	{
		cout<<"OPERATIONS :: 1.INSERT  2.DELETE  3.SEARCH  4.DISPLAY  5.SEARCH ALL KEY-VALUES FOR AN INTERVAL  6.GET ALL KEY-VALUES 7.QUIT"<<endl;
		cout<<"Enter Operation opcode : ";
		cin>>q;

		switch(q)
		{
		case 1:
			{
				int n,key,value;
				cout<<"Enter number of elements : ";
				cin>>n;
				for(int i=0;i<n;i++)
				{
					cout<<"Enter key value : ";
					cin>>key>>value;

					root = insert_node(root,key,value);
				}
				cout<<endl<<endl;

			}
			break;
		case 2:
			{
				if(root != NULL){
					int key;
					cout<<"Enter key to delete : ";
					cin>>key;
					delete_key(root,key);

					if(root->n == 0)
					{
						if(root->datanode == 0)
						{
							root = root->d.pivotmap[0];
						}else
						{

							root = NULL;
						}
					}
					//cout<<"key deleted successfully ..."<<endl<<endl;
					cout<<endl<<endl;
				}else
				{
					cout<<"Tree is empty"<<endl<<endl;
				}

			}
			break;
		case 3:
			{
				if(root != NULL)
				{
					int key;
					cout<<"Enter key which needs to be search : ";
					cin>>key;
					search(root,key);
					cout<<endl;
				}
				else
				{
					cout<<"Tree is empty"<<endl<<endl;
				}
			}
			break;
		case 4:
			{
				if(root != NULL)
				{
					display(root);
				}
				else
				{
					cout<<"Tree is empty"<<endl<<endl;
				}
			}
			break;
		case 5:
			if(root != NULL)
			{
				int start_key,end_key;
				cout<<"Enter starting and ending key for an interval  : "<<endl;
				cin>>start_key>>end_key;
				get_interval_values(root,start_key,end_key);
				cout<<endl<<endl;
			}
			else
			{
				cout<<"Tree is empty"<<endl<<endl;
			}
			break;
		case 6:
			if(root != NULL)
			{
				cout<<"ALL Key-value pairs : "<<endl;
				search_all(root);
				cout<<endl<<endl;
			}
			else
			{
				cout<<"Tree is empty"<<endl<<endl;
			}
			break;
		case 7:
			break;
		default:
			{
				cout<<"Enter valid opcode"<<endl;
			}
		}


	}while(q!=7);

	/*

	//int arr[]={3,18,14,30,32,36,15,16,12,40,45,38,39,11,6,10};
	//int arr[]={3,18,14,30,32,36,15,16,12,40,45,38,39,11,6,10,46,47,48};
	//int arr[19]={3,18,14,30,32,36,15,16,12,40,45,38,39,11,6,10,31,35,37};
	//int arr[]={3,18,14,30,32,36,15,16,12,40,45,38,39,11,6,10,46,47,48,14,31,19,20,24,23,21};
	int arr[]={3,18,14,30,32,36,15,16,12,40,45,38,39,11,6,10,46,47,48,17,31,19,20,24,23,21,37,41,25,26};
	for(int i=0;i<30;i++)
	{
	cout<<"INSERT : "<<arr[i]<<endl;

	if(root==NULL)
	{
	root= new node;
	root->datanode =1;
	root->key[0] = arr[i];
	root->d.value[0]=arr[i]+1;
	root->n =1;
	root->nextsib = NULL;

	}else
	{
	int result = insert(root,arr[i],arr[i]+1);
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
	display(root);
	cout<<endl;
	//cout<<endl<<endl<<"ALL KEY-VALUE PAIRS : "<<endl;
	//search_all(root);
	}

	cout<<endl<<endl<<"ALL KEY-VALUE PAIRS : "<<endl;
	search_all(root);
	cout<<endl<<endl;
	int key_1,key_2;
	cout<<"Enter key1 key2 : "<<endl;
	cin>>key_1>>key_2;
	get_interval_values(root,key_1,key_2) ;


	int key_;
	for(int i=0;i<30;i++)
	{
	cout<<"Enter key for delete : ";
	cin>>key_;
	delete_key(root,key_);

	if(root->n == 0)
	{


	//node*temp = root;
	if(root->datanode == 0)
	{
	root = root->d.pivotmap[0];
	}else
	{

	root = NULL;
	}
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

