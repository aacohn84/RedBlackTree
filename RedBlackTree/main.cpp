#include <iostream>
#include "RedBlackTree.h"
#include "RandomInt.h"

using namespace std;

void printVec(const vector<int> &v)
{
	auto i = v.begin();
	while (i != v.end())
		cout << *(i++) << " ";
	cout << endl;
}

void pressAnyKeyToQuit()
{
	while(isspace(cin.peek()))
		cin.get();
	cin.get();
}

int main()
{
	int numInts;
	cout << "Number of random integers to insert: ";
	cin >> numInts;

	RedBlackTree<int, double> tree;
	RandomInt keyGen(0, numInts * 2);
	RandomInt indexGen(0, numInts - 1);

	cout << "Generating random keys..." << endl;
	int i;
	for (i = 0; i < numInts; i++)
	{
		keyGen.getUnique();
		indexGen.getUnique();
	}

	vector<int> numbers = keyGen.history();
	vector<int> indices = indexGen.history();

	//cout << "numbers: "; printVec(numbers);
	//cout << "indices: "; printVec(indices);

	cout << "Doing tree inserts..." << endl;
	for (i = 0; i < numInts; i++)
	{
		int k = numbers[i];
		double v = k + 1.0;
		tree.insert(k, v);
	}

	cout << "Doing tree removals with error checking (may take awhile)..." << endl;

	bool error = false;
	for (i = 0; i < numInts; i++)
	{
		bool isBinary = tree.isBinary();
		bool isConnected = tree.isConnected();
		bool isRedBlack = tree.isRedBlackTree();
		error = error || !(isBinary && isConnected && isRedBlack);
		if (error)
			break;

		int index = indices[i];
		int n = numbers[index];
		tree.remove(n);
	}

	if (error)
		cout << "There was a problem during the " << i << "th removal.";
	else
		cout << "No errors occurred.";
	/*char choice;
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
	} while (choice != 'q');*/
	
	pressAnyKeyToQuit();
	return 0;
}