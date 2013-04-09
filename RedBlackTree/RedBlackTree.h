#pragma once

#include "util.h" // util::swap
#include <exception>

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

		/*	Returns the parent of the parent.
			Pre-cond: Parent must exist.	*/
		RedBlackNode* grandparent()
		{
			return parent->parent;
		}
		
		/*	Returns the parent's sibling.
			Pre-cond: Grandparent must exist.	*/
		RedBlackNode* uncle()
		{
			RedBlackNode *g = grandparent();
			return (parent == g->left ? g->right : g->left);
		}

		/*	Finds and returns the left-most node from the right sub-tree.
			Pre-cond: right child must exist.	*/
		RedBlackNode* successor()
		{
			RedBlackNode *s = this->right;
			
			while (s->left)
				s = s->left;

			return s;
		}

		/*	Right child becomes this node's parent, and this node becomes the
			left child.	
			Pre-cond: right child must exist.	*/
		void rotateLeft()
		{
			// the right node becomes this node's parent
			if (parent)
			{
				if (this == parent->left)
					parent->left = right;
				else
					parent->right = right;
			}
			right->parent = parent;
			parent = right;

			// the right node is replaced by its left child
			right = right->left;
			if (right)
				right->parent = this;

			// the new parent now has this node as its left child
			parent->left = this;
		}
		
		/*	Left child becomes this node's parent, and this node becomes the
			right child.	
			Pre-cond: left child must exist.	*/
		void rotateRight()
		{
			// the left node becomes this node's parent
			if (parent)
			{
				if (this == parent->right)
					parent->right = left;
				else
					parent->left = left;
			}
			left->parent = parent;
			parent = left;

			// right child of new parent becomes left child of this node
			left = left->right;
			if (left)
				left->parent = this;

			// this node becomes the right child of the new parent
			parent->right = this;
		}

		/*	This node is replaced by n, which may be a NULL pointer.	*/
		void replaceWith(RedBlackNode *n)
		{
			if (parent)
				(this == parent->left ? parent->left : parent->right) = n;

			if (n)
				n->parent = parent;
		}
		/*	out node is replaced by in node. out node is deleted. 
			in may be a NULL pointer.	*/
		static void transplant(RedBlackNode *out, RedBlackNode *in)
		{
			RedBlackNode *p = out->parent;

			if (p)
				(out == p->left ? p->left : p->right) = in;

			if (in)
				in->parent = p;
			
			delete out;
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
	
	/*	Inserts a node with the given key/value into the tree while	maintaining
		BST and red-black properties. Raises an exception if the key is already
		in the tree.	*/
	void insert(Key key, Value value) 
	{
		// find the position where the new node will be inserted
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
			else if (key > iter->key)
			{
				iter = iter->right;
				left = false;
			}
			else
				throw std::exception("Key already in tree.");
		}
		
		// create the new node and tie it into the tree
		RedBlackNode *newNode = new RedBlackNode(key, value, parent);
		if (!parent)
			root = newNode;
		else
			(left ? parent->left : parent->right) = newNode;

		// Perform local fixes to maintain red-black properties
		localFixCase1(newNode);
	}

	/*	Inserted at the root	*/
	void localFixCase1(RedBlackNode *n)
	{
		if (n == root)
			n->isBlack = true;
		else
			localFixCase2(n);
	}

	/*	Parent is black, no problem.	*/
	void localFixCase2(RedBlackNode *n)
	{
		if (n->parent->isBlack)
			return;
		else
			localFixCase3(n);
	}

	/* Parent and uncle are red.	*/
	void localFixCase3(RedBlackNode *n)
	{
		RedBlackNode *u = n->uncle();
		if (u != NULL && !u->isBlack)
		{
			n->parent->isBlack = true;
			u->isBlack = true;
			RedBlackNode *g = n->grandparent();
			g->isBlack = false;
			localFixCase1(g);
		}
		else
			localFixCase4(n);
	}

	/* Parent is red, uncle is black, path from n to g is not straight */
	void localFixCase4(RedBlackNode *n)
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
		localFixCase5(n);
	}

	/* Parent is red, uncle is black, path from n to g is straight */
	void localFixCase5(RedBlackNode *n)
	{
		RedBlackNode *g = n->grandparent();
		n->parent->isBlack = true;
		g->isBlack = false;
		if (n == n->parent->left)
			g->rotateRight();
		else
			g->rotateLeft();

		if (g == root)
			root = g->parent;
	}

	/*	Finds and removes the node with the specified key while maintaining the
		BST and red-black properties of the tree. Raises an exception if the
		node is not in the tree.	*/
	void remove(Key key) 
	{
		// find the node to be deleted
		RedBlackNode *mark = root;
		while (key != mark->key)
		{
			if (key < mark->key)
				mark = mark->left;
			else
				mark = mark->right;
		}
		
		if (!mark)
			throw std::exception("KeyError: key not found in tree.");

		if (!mark->left)
		{	// mark might have a right child
			mark->replaceWith(mark->right);
			delete mark;
		}
		else if (!mark->right)
		{	// mark definitely has a left child
			mark->replaceWith(mark->left);
			delete mark;
		}
		else
		{	// mark has two children
			RedBlackNode *s = mark->successor();
			
			// replace mark's data with successor's data
			mark->key = s->key;
			mark->value = s->value;

			s->replaceWith(s->right);
			delete s;
		}
	}

	Value* find(Key key) {}

	/*	Recursively deallocates every node in the tree using a post-order 
		traversal.	*/
	void deleteTree(RedBlackNode *t)
	{
		if (t)
		{
			deleteTree(t->left);
			deleteTree(t->right);
			delete t;
		}
	}

	/*	Verifies the red-black properties of the tree. Returns true if all of 
		the properties are maintained (making this a valid red-black tree).	*/
	bool isRedBlackTree() 
	{ 
		return root->isBlack && noSuccessiveReds() && goodBlackHeights();
	}

	/*	Verifies the "no successive reds" property of the tree: if a node is
		red, it's children must be black.
		Returns true if the property is maintained.	*/
	bool noSuccessiveReds()
	{
		bool inViolation = false;
		noSuccessiveReds(root, inViolation);
		return !inViolation;
	}

	/*	Recursively verifies the "no successive reds" property of the tree.
		Returns true if the property is violated.	*/
	void noSuccessiveReds(RedBlackNode *t, bool &inViolation)
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
			noSuccessiveReds(t->left, inViolation);
			noSuccessiveReds(t->right, inViolation);
		}
	}

	/*	Verifies the black height property of the tree: For each node, all 
		simple paths from the node to descendent leaves contain the same
		number of black nodes. 
		Returns true if the property is maintained.	*/
	bool goodBlackHeights()
	{
		bool inViolation = false;
		goodBlackHeights(root, inViolation);
		return !inViolation;
	}

	/*	Recursively verifies the black height property of the tree. 
		Returns true if the property is violated.	*/
	void goodBlackHeights(RedBlackNode *t, bool &inViolation)
	{
		if (!inViolation && t)
		{
			if (blackHeight(t) != -1)
			{
				goodBlackHeights(t->left, inViolation);
				goodBlackHeights(t->right, inViolation);
			}
			else
				inViolation = true;
		}
	}
	
	/*	Recursively measures the black height of the tree.
		Returns height if consistent, -1 if not. */
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

	/*	Verifies the binary search property of the tree. 
		Returns true if the property is maintained.	*/
	bool isBinary() { return isBinary(root); }

	/*	Recursively verifies the binary search property of the tree. 
		Returns true of the property is maintained.	*/
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

	/*	Verifies the "connectedness" of the tree. That is, every child points 
		back to its parent. 
		Returns true if every node points back to its parent.	*/
	bool isConnected() { return isConnected(root, NULL); }

	/*	Verifies the "connectedness" of the tree.
		Returns true if every node points back to its parent.	*/
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

	/*	Returns the depth of the tree. A tree of depth zero has one node. A 
		tree of depth -1 is empty.	*/
	int depth() { return depth(root); }

	/*	Recursively measures the depth of the tree.
		Returns the depth of the tree.	*/
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
