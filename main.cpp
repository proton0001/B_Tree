#include "BTree.h"

int main()
{
    BTree tree(2);
    int i, data;
    Node* root = NULL;
    Node* node = NULL;
    cout << "B-Tree Implementation\n";
    while(true)
    {
        cout << "\n1. Insert\n2. Delete a key.\n3. Search.\n4. Display inorder.\n5. Exit.\n";
        cin >> i;
        switch(i)
        {
            // Insert a key in Tree
            case 1:
                cout << "Enter key : ";
                cin >> data;
                tree.insert(data);
                break;
            // Delete a key from Tree
            case 2:
                cout << "Enter key to be deleted : ";
                cin >> data;
                tree.deleteNode(data);
                break;
            // Search a key in Tree
            case 3:
                cout << "Enter key to Search: ";
                cin >> data;
                tree.search(data);
                break;
            // Print key inorder
            case 4:
                tree.inorder();
                break;
            case 5:
                return 0;
            default:
                cout << "Invalid Response, Try Again :(\n";

        }
   }
}