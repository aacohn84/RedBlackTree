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

	void remove(Key key) 
	{
		/*
			find the node to be deleted and its parent
			replace the node to be deleted by its successor
			delete the node
		*/
	}

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

	bool isBinary() { return isBinary(root); }

	bool isBinary(RedBlackNode *t)
	{
		bool left_is_binary = true;
		bool right_is_binary = true;
	
		if (t->left)
		{
			if (t->left->value < t->value)
				left_is_binary = isBinary(t->left);
			else
				left_is_binary = false;
		}
		if (t->right)
		{
			if (t->right->value > t->value)
				right_is_binary = isBinary(t->right);
			else
				right_is_binary = false;
		}
		return (left_is_binary && right_is_binary);
	}

	bool isConnected() { return isConnected(root, NULL); }

	bool isConnected(RedBlackNode *t, RedBlackNode *parent)
	{
		if (t)
		{
			if (parent != NULL && t->parent != parent)
				return false;

			return isConnected(t->left, t) && isConnected(t->right, t);
		}
		return true;
	}

	int depth() { return depth(root); }

	int depth(RedBlackNode *t)
	{
		if (t)
		{
			int left_depth = depth(t->left);
			int right_depth = depth(t->right);

			return max(left_depth, right_depth) + 1;
		}
		return -1;
	}

};
