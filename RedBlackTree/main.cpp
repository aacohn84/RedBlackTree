/*
	Author: Aaron Cohn
	Date:	4/1/2013

	This is the driver program for testing the red-black tree.
*/

#include <iostream>
#include "RedBlackTree.h"
#include "RandomInt.h"

using namespace std;

void doRandomizedInsertsAndDeletes(RedBlackTree<int, double> &tree);
void doManualTreeInteraction(RedBlackTree<int, double> &tree);
void printVec(const vector<int> &v);

int main()
{
	RedBlackTree<int, double> tree;
	
	char choice;
	const char *message = 
		"How would you like to interact with the red-black tree?\n"
	    "R)andom insertions and deletions\n" 
		"M)anually\n" 
		"Q)uit ";
	do 
	{
		std::cout << message;
		cin >> choice;
		switch (choice = tolower(choice))
		{
			case 'r': doRandomizedInsertsAndDeletes(tree); break;
			case 'm': doManualTreeInteraction(tree); break;
			case 'q': std::cout << "Bye!" << endl; break;
			default:
				std::cout << "Choice not recognized.";
		}
		tree.clear();
	}
	while (choice != 'q');

	return 0;
}

void doRandomizedInsertsAndDeletes(RedBlackTree<int, double> &tree)
{
	int numInts;
	cout << "Preparing to do randomized insertions and deletions.\n"
			"How many random integers to use? ";
	cin >> numInts;
	
	RandomInt keyGen(0, numInts * 2);
	RandomInt indexGen(0, numInts - 1);

	cout << "\nGenerating random keys..." << endl;
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
		cout << "There was a problem during the " << i << "th removal.\n";
	else
		cout << "No errors occurred.\n";
	cout << endl;
}

void doManualTreeInteraction(RedBlackTree<int, double> &tree)
{
	char choice;
	do {
		cout << "A)dd, D)elete, C)lear, R)eturn: "; cin >> choice;
		switch (choice = tolower(choice))
		{
			int key;
			case 'a':
				cout << "Add key: ";
				cin >> key;
				tree.insert(key, key * 2.0);
				break;
			case 'd':
				cout << "Remove key: ";
				cin >> key;
				try { tree.remove(key); }
				catch (std::exception e) { cout << e.what(); }
				break;
			case 'c':
				tree.clear();
				break;
			case 'r':
				cout << endl;
				return;
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
	} while (choice != 'r');
	
	cout << endl;
}

void printVec(const vector<int> &v)
{
	auto i = v.begin();
	while (i != v.end())
		std::cout << *(i++) << " ";
	std::cout << endl;
}