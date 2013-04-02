#pragma once

#ifndef NULL
	#define NULL 0
#endif

template <class Key, class Value>
class RedBlackTree
{
private:
	class RedBlackNode
	{
	public:
		Key key;
		Value value;
		RedBlackNode *left, *right, *parent;
		bool isRed, isLeaf;
		int blackHeight;

		RedBlackNode(Key key, Value value, RedBlackNode *parent)
			: key(key), value(value), isRed(true), isLeaf(true),
			left(NULL), right(NULL), parent(parent)
		{
		}
	};

public:
	RedBlackNode *root;

	RedBlackTree()
		: root(NULL) 
	{
	}
	
	~RedBlackTree()
	{
		deleteTree(root);
	}
	
	void insert(Key key, Value value) 
	{
		root = insert(root, key, value);
	}

	RedBlackNode* insert(RedBlackNode *t, Key key, Value value)
	{
		if (!t)
			t = new RedBlackNode(key, value, NULL);
		else
		{
			if (key < t->key)
				t->left = insert(t->left, key, value);
			else
				t->right = insert(t->right, key, value);
		}
		return t;
	}

	void remove(Key key) {}
	Value* find(Key key) {}

	void deleteTree(RedBlackNode *t)
	{
		if (t)
		{
			deleteTree(t->left);
			deleteTree(t->right);
			delete t;
		}
	}
};
