#include <iostream>
#include "RedBlackTree.h"

using namespace std;

int main()
{
	RedBlackTree<int, double> tree;

	tree.insert(1, 2.0);
	tree.insert(2, 3.0);
	tree.insert(0, 1.0);

	return 0;
	// create red-black tree
	// while inserting random values into tree...
	//   test red-black properties
	// delete the tree
}