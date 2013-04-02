#include <iostream>
#include "RedBlackTree.h"

using namespace std;

int main()
{
	RedBlackTree<int, double> tree;

	tree.insert(1, 2.0);
	tree.insert(2, 3.0);
	tree.insert(0, 1.0);
	tree.insert(10, 9.0);
	tree.insert(7, 6.0);
	tree.insert(-1, -2.0);

	bool is_binary = tree.isBinarySearchTree();
	bool is_connected = tree.isConnectedTree();

	if (is_binary)
		cout << "Tree is binary." << endl;
	if (is_connected)
		cout << "Tree is connected." << endl;

	return 0;
	// create red-black tree
	// while inserting random values into tree...
	//   test red-black properties
	// delete the tree
}