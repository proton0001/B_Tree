#ifndef BTree_h
#define BTree_h

#include <iostream>

using namespace std;

class Node 
{
public:
	int n;  // No. of keys
	int *keys;
	bool isLeaf;
	Node **child;
};




class BTree 
{
private:
	int t;
	Node* root;

public:
	BTree(int t);
	Node* createNode();
	void insert(int key);
	void deleteNode(int key);
	void Delete(int val, Node *x);
	Node* search(Node* x, int key);
	void search(int key);
	void splitChild(Node* x, int i);
	void insertNonFull(Node* x, int key);
	void inorder();
	void inorder(Node* x);
};

#endif