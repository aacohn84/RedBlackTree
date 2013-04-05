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

	bool is_binary = tree.isBinary();
	bool is_connected = tree.isConnected();
	bool is_redblack = tree.isRedBlackTree();

	cout << "Tree is " << (is_binary ? "" : "not ") << "binary." << endl;
	cout << "Tree is " << (is_connected ? "" : "not ") << "connected." << endl;
	cout << "Tree is " << (is_redblack ? "" : "not ") << "redblack." << endl;
	cout << tree.depth() << endl;

	return 0;
}