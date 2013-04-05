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
			RedBlackNode *g = grandparent();
			return (parent == g->left ? g->right : g->left);
		}

		void rotateLeft()
		{
			// the right node becomes this node's parent
			if (this == parent->left)
				parent->left = right;
			else
				parent->right = right;
			right->parent = parent;
			parent = right;

			// the right node is replaced by its left child
			right = right->left;
			if (right)
				right->parent = this;

			// the new parent now has this node as its left child
			parent->left = this;
		}
		
		void rotateRight()
		{
			// the left node becomes this node's parent
			if (this == parent->right)
				parent->right = left;
			else
				parent->left = left;
			left->parent = parent;
			parent = left;

			// the left node is replaced by its right child
			left = left->right;
			if (left)
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

	bool isRedBlackTree() 
	{ 
		bool p2 = checkProperty2();
		bool p4 = checkProperty4();
		bool p5 = checkProperty5();

		return p2 && p4 && p5;
	}

	bool checkProperty2()
	{
		// root must be black
		return root->isBlack;
	}

	bool checkProperty4()
	{
		// if a node is red, it's children must be black
		bool inViolation = false;
		checkProperty4(root, inViolation);
		return !inViolation;
	}

	void checkProperty4(RedBlackNode *t, bool &inViolation)
	{
		if (!inViolation && t)
		{
			if (!t->isBlack)
			{	
				bool hasRedChild = t->left && !t->left->isBlack;
				hasRedChild |=  t->right && !t->right->isBlack;
				if (hasRedChild)
				{	// t is red and has at least 1 red child
					inViolation = true;
					return;
				}
			}
			// no errors yet, continue checking
			checkProperty4(t->left, inViolation);
			checkProperty4(t->right, inViolation);
		}
	}

	bool checkProperty5()
	{
		// For each node, all simple paths from the node to descendent leaves contain
		// the same number of black nodes
		bool inViolation = false;
		checkProperty5(root, inViolation);
		return !inViolation;
	}

	void checkProperty5(RedBlackNode *t, bool &inViolation)
	{
		if (!inViolation && t)
		{
			if (blackHeight(t) != -1)
			{
				checkProperty5(t->left, inViolation);
				checkProperty5(t->right, inViolation);
			}
			else
				inViolation = true;
		}
	}

	int blackHeight(RedBlackNode *t)
	{
		if (t)
		{
			int bhl = blackHeight(t->left);
			int bhr = blackHeight(t->right);
			if (bhl == -1 || bhr == -1)
				return -1;
			else 
			{
				if (!t->left || t->left->isBlack)
					bhl += 1;
				if (!t->right || t->right->isBlack)
					bhr += 1;

				if (bhl == bhr)
					return bhl;
				else
					return -1;
			}
		}
		return 0;
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
