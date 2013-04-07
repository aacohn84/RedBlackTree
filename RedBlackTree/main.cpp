#include <iostream>
#include "RedBlackTree.h"
#include "RandomInt.h"

using namespace std;

int main()
{
	RedBlackTree<int, double> tree;
	RandomInt generator(0, 1000);

	for (int i = 0; i < 100; i++)
	{
		int r = generator.get();
		try {
			tree.insert(r, r + 1.0);
		} catch (exception e) { e.what(); }
	}
	bool is_binary = tree.isBinary();
	bool is_connected = tree.isConnected();
	bool is_redblack = tree.isRedBlackTree();

	cout << "Tree is " << (is_binary ? "" : "not ") << "binary." << endl;
	cout << "Tree is " << (is_connected ? "" : "not ") << "connected." << endl;
	cout << "Tree is " << (is_redblack ? "" : "not ") << "redblack." << endl;
	cout << "Depth is " << tree.depth() << endl;

	return 0;
}