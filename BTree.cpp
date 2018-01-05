#include "BTree.h"

BTree::BTree(int t) {
    this->t = t;
    root = createNode();
    root->isLeaf = true;
    root->n = 0;
}

Node* BTree::createNode() { // Creates an empty node
    Node* node = new Node;
    node->keys = new int[2*t-1];
    node->child = new Node*[2*t];
    for(int i = 0; i < 2*t; i++)
        node->child[i] = NULL;

    return node;
}

    // Splits the ith child in the node x. And moves the median key up. 
void BTree::splitChild(Node* x, int i)
{
    Node * z = createNode();    // The other half of node after split.
    Node * y = x->child[i];
    z->isLeaf = y->isLeaf;
    z->n = t - 1;
    for(int j = 0; j < t-1; j++)    // Copying the keys from t to t-1 from y to z.
        z->keys[j] = y->keys[j+t];
    if (y->isLeaf == false)
        for(int j = 0; j < t; j++)  // Copying the children
            z->child[j] = y->child[j+t];
    y->n = t-1;
    for(int j = x->n; j >= i+1; j--)
        x->child[j+1] = x->child[j];    // Shifting x's child from [i+2, n] by 1.
    x->child[i+1] = z;
    for(int j = x->n-1; j >= i; j--)    // Making place for the new key in the parent x.
        x->keys[j+1] = x->keys[j];
    x->keys[i] = y->keys[t-1];  // Adding the new key at apt position.
    x->n += 1;
}


void BTree::insertNonFull(Node *x, int k) {
    int i = x->n;
    if (x->isLeaf)
    {
        while(i >= 1 && k < x->keys[i-1]) {
            x->keys[i] = x->keys[i-1];
            i = i - 1;
        }
        x->keys[i] = k;
        x->n = x->n + 1;
    }
    else  {
        while (i >= 1 && k < x->keys[i-1]) 
            i--;
        i++;
        // Making sure sure the child is not full. If it is, split it. So that any further splits will have the parent as non full.
        if(x->child[i-1]->n == 2*t-1) 
        { 
            splitChild(x, i-1);
            if(k > x->keys[i-1])
                i++;
        }
        insertNonFull(x->child[i-1], k);
    }
}
// Prints all keys in a node
void printNode(Node *x)
{    
    for(int i = 0; i < x->n; i++)
        cout << x->keys[i] << " ";
    cout << endl;
}
// Inserts the key.
void BTree::insert(int key)
{
    Node *r = root;
    // If the root is full, split it.
	if (root->n == 2*t-1) 
	{ 
	    Node * s = createNode();
	    root = s;
	    s->isLeaf = false;
	    s->n = 0;
	    s->child[0] = r;
	    splitChild(s, 0);
	    insertNonFull(s, key);
	}
	else
	    insertNonFull(r, key);
}

void BTree::inorder()
{
    cout << endl;
    inorder(root);    
}

Node* BTree::search(Node* x, int key) 
{
    if(x) 
    {
        int y = 0;
        for(int i = 0; i < x->n; i++)
        {
            if(key >= x->keys[i])
                y = i+1; 
        }
        if(x->keys[y-1] == key)
            return x;   
        return search(x->child[y], key);
    }
    return NULL;

}

void BTree::search(int key)
{
    if(search(root, key))
        cout << "The key was found."<<endl;
    else
        cout << "Couldn't find the key"<<endl;
}

// Printss the tree inorder
void BTree::inorder(Node * x)
{ 
    if(x != NULL)
    {
    	// No. of keys. Therefore, children = n+1
        int n = x->n;   
        for(int i = 0; i < n; i++)
        {
            inorder(x->child[i]);
            cout << x->keys[i] <<" ";
        }
        cout << endl;
        inorder(x->child[n]);
    }
}

//delete val, we will adjust later
void popFrom(Node *x, int val)
{
	int i,j;
	if(x->isLeaf && x->n>=2)
	{
		for(i=0;i<x->n;i++)
		{
			if(val==x->keys[i])
			{
				for(j=i;j< x->n - 1;j++)
				{
					x->keys[j]=x->keys[j+1];
				}
				x->n -= 1;;
				break;
			}
		}
	}
}

//merges child[loc+1] and key[loc] into child[loc] and deletes it from parent.
void merge(Node *x, int loc)
{
	int i,j;
	Node *y = x->child[loc];
	Node *z = x->child[loc+1];
	y->keys[y->n] = x->keys[loc];
	for(i=0;i< z->n;i++)
		y->keys[y->n+i+1] = z->keys[i];
	if (y->isLeaf==false)
	{
		for(i=0; i<=z->n; i++)
			y->child[y->n+i+1] = z->child[i];
	}
	for (i=loc+1; i<x->n ;i++)
		x->keys[i-1] = x->keys[i];
	for (i=loc+2; i<=x->n ;i++)
		x->child[i-1] = x->child[i];
	y->n = y->n + z->n + 1;
	x->n -= 1;
	delete(z);
}

Node* predecessor(Node* x, int i) // Returns immediate inorder predecessor
{
	Node* ptr=x->child[i];
	while(!ptr->isLeaf)
		ptr=ptr->child[ptr->n];
	return ptr;
}


Node* successor(Node *x, int i) // Returns immediate successor
{
		Node* ptr = x->child[i+1];
	while(!ptr->isLeaf)
		ptr=ptr->child[0];
	return ptr;
}


void borrowFromPrev(Node *x, int loc) // borrows from left sibling along with its child
{
	Node* y=x->child[loc];
	Node* z=x->child[loc-1];
	// making place in y
	for(int i=y->n-1; i>=0; i--)
		y->keys[i+1]=y->keys[i];
		// making place in y in children
	if(!y->isLeaf)
	{
		for(int i=y->n; i>=0; i--)
		y->child[i+1] = y->child[i];
	}
	y->keys[0]=x->keys[loc-1];
	if(!z->isLeaf)
		y->child[0] = z->child[z->n];
	x->keys[loc-1]=z->keys[z->n-1];
	y->n=y->n+1;
	z->n=z->n-1;
}
void borrowFromNext(Node *x, int loc) // Borrows from right sibling along with its child
{
	Node* y=x->child[loc];
	Node* z=x->child[loc+1];
	y->keys[y->n]=x->keys[loc];
	if(!z->isLeaf)
		y->child[(y->n)+1] = z->child[0];
	x->keys[loc]=z->keys[0];
	
	// Shifting all keys in z one behind
	for(int i=0; i<z->n-1; i++)
		z->keys[i]=z->keys[i+1];
		// Shifting all children in z one behind
	if(!z->isLeaf)
	{
		for(int i=0; i<z->n; i++)
			z->child[i]=z->child[i+1];
	}
	y->n=y->n+1;
	z->n=z->n-1;
}
// Delete Node from B-Tree
void BTree::deleteNode(int key)
{
    Delete(key, root);
}
// Deletes the key val in subtree x.
void BTree::Delete(int val, Node* x)
{ 
	int i,loc;
    int m = t - 1;
	Node* y= search(x, val);
	if(y!=NULL)
	{
		if(y->isLeaf&&y==root)  // Delete directly from root.
		{
			popFrom(root, val);
			if(root->n==0)
				root=NULL;
		}
		else if(y->isLeaf&&y->n>m) // Leaf with more than t - 1 keys.
			popFrom(y, val);
		else if(y->isLeaf)   // Deleting from leaf with less than t keys.
		{
			popFrom(y, val);
			while(y->n==1) 
			{
				int i;
				Node *ptr=root;// Finds parent and y's parent.
				Node *prev=NULL;
				Node *parent=NULL;
				while(ptr->isLeaf == false)
				{
					for(i = 0; i < (ptr->n); i++)
					{
					  	if(y->keys[0]< ptr->keys[0])
						{
							i=0;
							break;
						}
						else if((y->keys[0] > ptr->keys[i])&&(y->keys[0] < ptr->keys[i+1]))
						{
							i++;
							break;
						}
					}
					prev=ptr;	
					ptr = ptr->child[i];
					if(ptr==y)
					{
						parent=prev;
						break;
					}
				} // Find the parent and y's location.

				if(parent==NULL) // Root reached.
					break;
				if((i!=0)&&(parent->child[i-1])->n > m)   // Borrow from left sibling.
				{
					borrowFromPrev(parent, i);
				}
				else if((i!=parent->n)&&(parent->child[i+1])->n > m)    // Borrow from right sibling.
				{
					borrowFromNext(parent, i);
				}
				else   
				{
					if(i!=parent->n)
						merge(parent, i);
					else
						merge(parent, i-1);
				}
				y=parent;
			}
			
		}
		else if(y->isLeaf==false)   // If a non leaf
		{
			for(i=0;i<y->n;i++)
			{
				if(y->keys[i]==val)
				{
					loc=i;
					break;
				}		
			}
			if(y->child[loc+1]->n>m)
			{
				Node* s=successor(y, loc);
				// Replace with immediate successor and delete from it.
				y->keys[loc]=s->keys[0];
				Delete(s->keys[0],s);
			}
			else if(y->child[loc]->n>m)
			{
				Node* p= predecessor(y, loc);
				// Replace with pred and delete from it.
				y->keys[loc]=p->keys[p->n-1];
				Delete(p->keys[p->n-1],p);
			}
			else
			{
				int val=y->keys[loc]; // Merge and delete.
				merge(y, loc);
				Delete(val,root);
			}
		}
	}	
}