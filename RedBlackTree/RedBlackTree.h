#pragma once

#include "util.h" // util::swap

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
		bool isBlack;
		
		RedBlackNode(Key key, Value value, RedBlackNode *parent)
			: key(key), value(value), isBlack(false),
			left(NULL), right(NULL), parent(parent)
		{
		}

		RedBlackNode* grandparent()
		{
			return parent->parent;
		}
		
		RedBlackNode* uncle()
		{
			return (this == grandparent()->left ? parent->right : parent->left);
		}

		void rotateLeft()
		{
			// the right node becomes this node's parent
			right->parent = parent;
			parent = right;

			// the right node is replaced by its left child
			right = right->left;
			right->parent = this;

			// the new parent now has this node as its left child
			parent->left = this;
		}
		
		void rotateRight()
		{
			// the left node becomes this node's parent
			left->parent = parent;
			parent = left;

			// the left node is replaced by its right child
			left = left->right;
			left->parent = this;

			// the new parent now has this node as its right child
			parent->right = this;
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
		RedBlackNode *iter = root;
		RedBlackNode *parent = NULL;
		bool left = false;
		while (iter)
		{
			parent = iter;
			if (key < iter->key)
			{
				iter = iter->left;
				left = true;
			}
			else
			{
				iter = iter->right;
				left = false;
			}
		}
		
		RedBlackNode *newNode = new RedBlackNode(key, value, parent);
		if (!parent)
			root = newNode;
		else
			(left ? parent->left : parent->right) = newNode;

		insertCase1(newNode);
	}

	// case 1: inserted at the root
	void insertCase1(RedBlackNode *n)
	{
		if (n == root)
			n->isBlack = true;
		else
			insertCase2(n);
	}

	// case 2: parent is black, do nothing
	void insertCase2(RedBlackNode *n)
	{
		if (n->parent->isBlack)
			return;
		else
			insertCase3(n);
	}

	// case 3: parent and uncle are red
	void insertCase3(RedBlackNode *n)
	{
		RedBlackNode *u = n->uncle();
		if (u != NULL && !u->isBlack)
		{
			n->parent->isBlack = true;
			u->isBlack = true;
			RedBlackNode *g = n->grandparent();
			g->isBlack = false;
			insertCase1(g);
		}
		else
			insertCase4(n);
	}

	// parent is red, uncle is black, path from n to g is not straight
	void insertCase4(RedBlackNode *n)
	{
		RedBlackNode *g = n->grandparent();
		if (n == n->parent->right && n->parent == g->left)
		{
			n->parent->rotateLeft();
			n = n->left;
		}
		else if (n == n->parent->left && n->parent == g->right)
		{
			n->parent->rotateRight();
			n = n->right;
		}
		insertCase5(n);
	}

	// parent is red, uncle is black, path from n to g is straight
	void insertCase5(RedBlackNode *n)
	{
		RedBlackNode *g = n->grandparent();
		n->parent->isBlack = true;
		if (n == n->parent->left)
			g->rotateRight();
		else
			g->rotateLeft();

		if (g == root)
			root = g->parent;
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
