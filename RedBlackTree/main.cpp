#include <iostream>
#include "RedBlackTree.h"
#include "RandomInt.h"

using namespace std;

int main()
{
	RedBlackTree<int, double> tree;
	/*for (int i = 0; i < 3; i++)
		tree.insert(i, i * 2.0);*/
	/*
	RandomInt generator(0, 1000);

	for (int i = 0; i < 100; i++)
	{
		int r = generator.get();
		try {
			tree.insert(r, r + 1.0);
		} catch (exception e) { e.what(); }
	}*/

	char choice;

	do {
		cout << "A)dd, R)emove, C)lear, Q)uit: "; cin >> choice;
		switch (choice = tolower(choice))
		{
			int key;
			case 'a':
				cout << "Add key: ";
				cin >> key;
				tree.insert(key, key * 2.0);
				break;
			case 'r':
				cout << "Remove key: ";
				cin >> key;
				try { tree.remove(key); }
				catch (std::exception e) { cout << e.what(); }
				break;
			case 'c':
				tree.clear();
				break;
			case 'q': 
				break;
			default:
				cout << "Choice not recognized." << endl;
		} 
		bool is_binary = tree.isBinary();
		bool is_connected = tree.isConnected();
		bool is_redblack = tree.isRedBlackTree();

		cout << "Tree is " << (is_binary ? "" : "not ") << "binary." << endl;
		cout << "Tree is " << (is_connected ? "" : "not ") << "connected." << endl;
		cout << "Tree is " << (is_redblack ? "" : "not ") << "redblack." << endl;
		cout << "Depth is " << tree.depth() << endl;
	} while (choice != 'q');

	return 0;
}