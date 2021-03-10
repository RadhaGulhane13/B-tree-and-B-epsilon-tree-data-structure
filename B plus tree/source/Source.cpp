#include "B+ Tree.h"
#include <chrono> 
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
using namespace std::chrono; 


//insert , delete , search given no. of element
void test_case_1(node* root)
{

	//int arr_n = 100000;
	int* arr_s = new int[arr_n];

	cout<<"TEST CASE FOR "<<arr_n<<" element insert delete with ORDER "<<MAX_SIZE<<endl;

	for(int i=0;i<arr_n;i++){
		arr_s[i]=i;
	}

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	shuffle (arr_s, arr_s+arr_n, std::default_random_engine(seed));
	cout<<"INSERTING...";
	for(int i=0;i<arr_n;i++){
		//cout<<"insert:"<<arr_s[i]<<endl;
		root = insert_node(root,arr_s[i],arr_s[i]+1);
		if(!check_invarient(root)){
			cout<<"Invarient check finish with violation ";
		}

	}

	cout<<"INSERTION is Done"<<endl;
	cout<<"INVARIENT CHECKING... ";
	check_invarient(root)?cout<<"Invarient check finish without violation ":cout<<"Invarient check finish with violation ";
	cout<<"INVARIENT CHECKING is done"<<endl;

	//display(root);
	cout<<"SEARCHING...";
	for(int i=0;i<arr_n;i++){
		search(root,i);
	}
	cout<<"SEARCH is Done"<<endl;
	cout<<"DELETING...";
	for(int i=0;i<arr_n-1;i++){
		root = delete_node(root,i);
		if(!check_invarient(root)){
			cout<<"Invarient check finish with violation ";
		}

	}
	cout<<"DELETION is Done"<<endl;
	cout<<"INVARIENT CHECKING... ";
	check_invarient(root)?cout<<"Invarient check finish without violation ":cout<<"Invarient check finish with violation ";
	cout<<"INVARIENT CHECKING is done"<<endl;
	display(root);	
	delete[] arr_s;

}

//perform operation by giving inputs
void test_case_2(node* root)
{

	//OPERATIONS :1.INSERT  2.DELETE  3.SEARCH  4.DISPLAY 5.SEARCH ALL KEY-VALUES FOR AN INTERVAL  6.GET ALL KEY-VALUES
	int q;

	do {
		cout<<"OPERATIONS :: 1.INSERT  2.DELETE  3.SEARCH  4.DISPLAY  5.SEARCH ALL KEY-VALUES FOR AN INTERVAL  6.GET ALL KEY-VALUES 7.QUIT"<<endl;
		cout<<"Enter Operation opcode : ";
		cin>>q;

		switch(q) {
		case 1: {
			int n,key,value;
			cout<<"Enter number of elements : ";
			cin>>n;
			for (int i=0; i < n; i++) {
				cout<<"Enter key value : ";
				cin>>key>>value;

				root = insert_node(root,key,value);
			}
			cout<<endl<<endl;
			}
			break;
		case 2: {
			if(root != NULL) {
				int key;
				cout<<"Enter key to delete : ";
				cin>>key;
				delete_node(root,key);

				cout<<endl<<endl;
			} else {
				cout<<"Tree is empty"<<endl<<endl;
			}
			}
			break;
		case 3: {
			if (root != NULL) {
				int key;
				cout<<"Enter key which needs to be search : ";
				cin>>key;
				search(root,key);
				cout<<endl;
			} else {
				cout<<"Tree is empty"<<endl<<endl;
			}
			}
			break;
		case 4: {
			if (root != NULL) {
				display(root);
			} else {
				cout<<"Tree is empty"<<endl<<endl;
			}
			}
			break;
		case 5:
			if (root != NULL) {
				int start_key,end_key;
				cout<<"Enter starting and ending key for an interval  : "<<endl;
				cin>>start_key>>end_key;
				get_interval_values(root,start_key,end_key);
				cout<<endl<<endl;
			} else {
				cout<<"Tree is empty"<<endl<<endl;
			}
			break;
		case 6:
			if (root != NULL) {
				cout<<"ALL Key-value pairs : "<<endl;
				search_all(root);
				cout<<endl<<endl;
			} else {
				cout<<"Tree is empty"<<endl<<endl;
			}
			break;
		case 7:
			break;
		default:
			cout<<"Enter valid opcode"<<endl;

		}

		check_invarient(root)?cout<<"Invarient check finish without violation ":cout<<"Invarient check finish with violation ";

	}while(q!=7);

}

int main()
{


	/*
	cout<<"INTERNAL_NODE_SIZE : "<<INTERNAL_NODE_ORDER<<endl; 
	cout<<"DATA_NODE_SIZE : "<<DATA_NODE_ORDER<<endl;
	cout<<"ORDER : "<<ORDER<<endl;
	cout<<"B PLUS TREE V5"<<endl;

	cout<<"NODE_SIZE:"<<sizeof(node)<<endl;*/
	cout<<"*****************B PLUS TREE V7******************"<<endl;
	node *root=NULL;

	auto start = high_resolution_clock::now(); 
	test_case_1(root);
	auto stop = high_resolution_clock::now(); 

	auto duration = duration_cast<microseconds>(stop - start); 

	cout << "Time taken by B plus Tree V5: "
		<< duration.count() << " microseconds" << endl; 





}