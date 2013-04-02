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
		root = insert(root, key, value, NULL);
	}

	RedBlackNode* insert(RedBlackNode *t, Key key, Value value, RedBlackNode *parent)
	{
		if (!t)
			t = new RedBlackNode(key, value, parent);
		else
		{
			if (key < t->key)
				t->left = insert(t->left, key, value, t);
			else
				t->right = insert(t->right, key, value, t);
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

	bool isBinarySearchTree() { return isBinarySearchTree(root); }

	bool isBinarySearchTree(RedBlackNode *t)
	{
		bool left_is_binary = true;
		bool right_is_binary = true;
	
		if (t->left)
		{
			if (t->left->value < t->value)
				left_is_binary = isBinarySearchTree(t->left);
			else
				left_is_binary = false;
		}
		if (t->right)
		{
			if (t->right->value > t->value)
				right_is_binary = isBinarySearchTree(t->right);
			else
				right_is_binary = false;
		}
		return (left_is_binary && right_is_binary);
	}

	bool isConnectedTree() { return isConnectedTree(root, NULL); }

	bool isConnectedTree(RedBlackNode *t, RedBlackNode *parent)
	{
		if (t)
		{
			if (parent != NULL && t->parent != parent)
				return false;

			return isConnectedTree(t->left, t) && isConnectedTree(t->right, t);
		}
		return true;
	}

};
