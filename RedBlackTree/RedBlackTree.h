/*
	Author: Aaron Cohn

	A C++ template implementation of a Red-Black Tree
*/

#pragma once

#include <cassert>
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
		typedef RedBlackNode* Ptr;

		Key key;
		Value value;
		Ptr left, right, parent;
		bool isBlack;
		
		RedBlackNode(Key key, Value value, Ptr parent)
			: key(key), value(value), isBlack(false),
			left(NULL), right(NULL), parent(parent)
		{
		}

		bool isRed() { return !isBlack;	}

		/*	Returns a reference to the parent's pointer to this node.
			Pre-cond: Parent must exist.	*/
		Ptr& ptrFromParent() const
		{
			return (this == parent->left ? parent->left : parent->right);
		}

		/*	Returns the parent of the parent.
			Pre-cond: Parent must exist.	*/
		Ptr grandparent() const
		{
			return parent->parent;
		}
		
		/*	Returns the parent's sibling.
			Pre-cond: Grandparent must exist.	*/
		Ptr uncle() const
		{
			Ptr g = grandparent();
			return (parent == g->left ? g->right : g->left);
		}

		/*	Returns this node's sibling.
			Pre-cond: Parent must exist	*/
		Ptr sibling() const
		{
			if (this == parent->left)
				return parent->right;
			else
				return parent->left;
		}

		/*	Finds and returns the left-most node from the right sub-tree.
			Pre-cond: right child must exist.	*/
		Ptr successor() const
		{
			Ptr s = this->right;
			
			while (s->left)
				s = s->left;

			return s;
		}

		/*	Right child becomes this node's parent, and this node becomes the
			left child.	
			Pre-cond: right child must exist.	*/
		void rotateLeft()
		{
			// right child is promoted to this node's position in the tree
			if (parent)
				ptrFromParent() = right;

			// this node is demoted, its parent is now its former right child
			right->parent = parent;
			parent = right;

			// the new parent gives its left child to this node
			right = parent->left;
			if (right)
				right->parent = this;

			// this node becomes the left child of its new parent
			parent->left = this;
		}
		
		/*	Left child becomes this node's parent, and this node becomes the
			right child.	
			Pre-cond: left child must exist.	*/
		void rotateRight()
		{
			// left child is promoted to this node's position in the tree
			if (parent)
				ptrFromParent() = left;

			// this node is demoted, its parent is now its former left child
			left->parent = parent;
			parent = left;

			// the new parent gives its right child to this node
			left = parent->right;
			if (left)
				left->parent = this;

			// this node becomes the right child of its new parent
			parent->right = this;
		}

		/*	This node is replaced by n, which may be a NULL pointer.	*/
		void replaceWith(Ptr n) const
		{
			if (parent) 
				// n takes this node's place as its parent's left or right child
				ptrFromParent() = n;
			if (n)
				n->parent = parent;
		}
	};

public:
	RedBlackNode *root;

	RedBlackTree()
		: root(NULL) 
	{
	}
	
	~RedBlackTree() { deleteTree(root);	}
	
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
		insertFixCase1(newNode);
	}

	/*	Inserted at the root	*/
	void insertFixCase1(RedBlackNode *n)
	{
		if (n == root)
			n->isBlack = true;
		else
			insertFixCase2(n);
	}

	/*	Parent is black, no problem.	*/
	void insertFixCase2(RedBlackNode *n)
	{
		if (n->parent->isBlack)
			return;
		else
			insertFixCase3(n);
	}

	/* Parent and uncle are red.	*/
	void insertFixCase3(RedBlackNode *n)
	{
		RedBlackNode *u = n->uncle();
		if (u != NULL && u->isRed())
		{
			n->parent->isBlack = true;
			u->isBlack = true;
			RedBlackNode *g = n->grandparent();
			g->isBlack = false;
			insertFixCase1(g);
		}
		else
			insertFixCase4(n);
	}

	/* Parent is red, uncle is black, path from n to g is not straight */
	void insertFixCase4(RedBlackNode *n)
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
		insertFixCase5(n);
	}

	/* Parent is red, uncle is black, path from n to g is straight */
	void insertFixCase5(RedBlackNode *n)
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
		RedBlackNode *mark = find(key);
		if (!mark)
			throw std::exception("KeyError: key not found in tree.");
		remove(mark);
	}

	/*	Given the address of a node, removes it from the tree.
		Pre-cond: mark exists	*/
	void remove(RedBlackNode *mark)
	{
		if (mark->left && mark->right)
		{	// mark has two children
			RedBlackNode *s = mark->successor();
			
			// replace mark's data with successor's data
			mark->key = s->key;
			mark->value = s->value;

			mark = s;
		}

		// mark has at most one non-leaf (non-null) child
		redBlackRemoval(mark);
	}

	void redBlackRemoval(RedBlackNode *mark)
	{
		RedBlackNode *child = mark->left ? mark->left : mark->right;
		if (mark->isBlack)
		{
			if (child && child->isRed())
			{
				child->isBlack = true;
				if (mark == root)
					root = child;
			}
			else
				doubleBlackFix(mark);
		}
		mark->replaceWith(child);
		if (mark == root)
			root = NULL;
		delete mark;
	}

	void doubleBlackFix(RedBlackNode *mark)
	{
		removeFixCase1(mark);
	}

	/* Mark is the new root */
	void removeFixCase1(RedBlackNode *mark)
	{
		if (mark->parent)
			removeFixCase2(mark);
	}

	/* Sibling is red */
	void removeFixCase2(RedBlackNode *mark)
	{
		RedBlackNode *sibling = mark->sibling();

		if (sibling->isRed())
		{
			// swap the colors of sibling and parent
			mark->parent->isBlack = false;
			sibling->isBlack = true;

			// rotate about the parent
			if (mark == mark->parent->left)
				mark->parent->rotateLeft();
			else
				mark->parent->rotateRight();

			if (mark->parent == root)
				root = mark->grandparent();
		}
		removeFixCase3(mark);
	}

	/*	Node to be spliced is black, child is black	*/
	void removeFixCase3(RedBlackNode *mark)
	{
		RedBlackNode *sibling = mark->sibling();
		if (sibling)
		{
			bool childrenAreBlack;
			bool leftIsBlack = !sibling->left || sibling->left->isBlack;
			bool rightIsBlack = !sibling->right || sibling->right->isBlack;
			childrenAreBlack = leftIsBlack && rightIsBlack;

			if (mark->parent->isBlack && sibling->isBlack && childrenAreBlack)
			{
				sibling->isBlack = false;
				removeFixCase1(mark->parent);
			}
			else
				removeFixCase4(mark, sibling, childrenAreBlack);
		}
	}

	void removeFixCase4(RedBlackNode *mark, RedBlackNode *sibling, 
		bool childrenAreBlack)
	{
		if (mark->parent->isRed() && sibling->isBlack && childrenAreBlack)
		{
			sibling->isBlack = false;
			mark->parent->isBlack = true;
		}
		else
			removeFixCase5(mark, sibling);
	}

	void removeFixCase5(RedBlackNode *mark, RedBlackNode *sibling)
	{
		if (sibling->isBlack)
		{
			bool rightIsBlack = !sibling->right || sibling->right->isBlack;
			bool leftIsRed = sibling->left && sibling->left->isRed();
			if (mark == mark->parent->left && rightIsBlack && leftIsRed)
			{
				sibling->isBlack = false;
				sibling->left->isBlack = true;
				sibling->rotateRight();
			}
			else if (mark == mark->parent->right && !leftIsRed && 
				sibling->right && sibling->right->isRed())
			{
				sibling->isBlack = false;
				sibling->right->isBlack = true;
				sibling->rotateLeft();
			}
		}
		removeFixCase6(mark);
	}

	void removeFixCase6(RedBlackNode *mark)
	{	// fixCase1 guarantees that mark->parent exists
		RedBlackNode *sibling = mark->sibling();
		sibling->isBlack = mark->parent->isBlack;
		mark->parent->isBlack = true;

		if (mark == mark->parent->left)
		{
			if (sibling->right)
				sibling->right->isBlack = true;
			mark->parent->rotateLeft();
		}
		else
		{
			if (sibling->left)
				sibling->left->isBlack = true;
			mark->parent->rotateRight();
		}
		if (mark->parent == root)
			root = mark->grandparent();
	}

	RedBlackNode* find(Key key) const 
	{
		RedBlackNode *mark = root;
		while (mark && key != mark->key)
		{
			if (key < mark->key)
				mark = mark->left;
			else
				mark = mark->right;
		}
		return mark;
	}

	void clear()
	{
		deleteTree(root);
		root = NULL;
	}

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
	bool isRedBlackTree() const
	{ 
		if (root)
			return root->isBlack && noSuccessiveReds() && goodBlackHeights();
		else 
			return true;
	}

	/*	Verifies the "no successive reds" property of the tree: if a node is
		red, it's children must be black.
		Returns true if the property is maintained.	*/
	bool noSuccessiveReds() const
	{
		bool inViolation = false;
		noSuccessiveReds(root, inViolation);
		return !inViolation;
	}

	/*	Recursively verifies the "no successive reds" property of the tree.
		Returns true if the property is violated.	*/
	void noSuccessiveReds(RedBlackNode *t, bool &inViolation) const
	{
		if (!inViolation && t)
		{
			if (t->isRed())
			{	
				bool hasRedChild = t->left && t->left->isRed();
				hasRedChild |=  t->right && t->right->isRed();
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
	bool goodBlackHeights() const
	{
		bool inViolation = false;
		goodBlackHeights(root, inViolation);
		return !inViolation;
	}

	/*	Recursively verifies the black height property of the tree. 
		Returns true if the property is violated.	*/
	void goodBlackHeights(RedBlackNode *t, bool &inViolation) const
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
	int blackHeight(RedBlackNode *t) const
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
	bool isBinary() const
	{ 
		bool error = false;
		if (root)
			isBinary(root, error);
		return !error;
	}

	/*	Recursively verifies the binary search property of the tree. 
		Returns true of the property is maintained.	*/
	void isBinary(RedBlackNode *t, bool &error) const
	{
		if (t && !error)
		{
			bool left_lt = !t->left || t->left && t->left->key < t->key;
			bool right_gt = !t->right || t->right && t->right->key > t->key;
			if (left_lt && right_gt)
			{
				isBinary(t->left, error);
				isBinary(t->right, error);
			}
			else
				error = true;
		}
	}

	/*	Verifies the "connectedness" of the tree. That is, every child points 
		back to its parent. 
		Returns true if every node points back to its parent.	*/
	bool isConnected() const 
	{ 
		bool error = false;
		if (root)
			isConnected(root, NULL, error);
		return !error; 
	}

	/*	Verifies the "connectedness" of the tree.
		Returns true if every node points back to its parent.	*/
	void isConnected(RedBlackNode *t, RedBlackNode *parent, bool &error) const
	{
		if (t && !error)
		{
			if (parent != NULL && t->parent != parent)
				error = true;
			else
			{
				isConnected(t->left, t, error);
				isConnected(t->right, t, error);
			}
		}
	}

	/*	Returns the depth of the tree. A tree of depth zero has one node. A 
		tree of depth -1 is empty.	*/
	int depth() const { return depth(root); }

	/*	Recursively measures the depth of the tree.
		Returns the depth of the tree.	*/
	int depth(RedBlackNode *t) const
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
