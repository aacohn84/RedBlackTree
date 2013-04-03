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

	void rotation_RR(RedBlackNode *root)
	{
		// right heavy tree
		// right child takes the place of root
		// nodes shift to the left

		// hold onto nodes which might be axed during rotation
		RedBlackNode *old_left = root->left;
		RedBlackNode *pivot = root->right;
		RedBlackNode *pivot_left = pivot->left;

		// reassign pivot to root->left, reattach old left
		root->left = pivot;
		root->left->left = old_left;
		if (old_left)
			old_left->parent = root->left;

		// reassign pivot->right to root->right
		root->right = pivot->right;
		root->right->parent = root;

		// pivot's left child goes to root->left->right
		root->left->right = pivot_left;

		swap(&(root->left->value), &(root->value), sizeof(root->value));
		// pivot->left becomes root->left->max??
	}

	void rotation_LL(RedBlackNode *root)
	{
		// hold onto nodes which might be axed during rotation
		RedBlackNode *old_right = root->right;
		RedBlackNode *pivot = root->left;
		RedBlackNode *pivot_right = pivot->right;

		// reassign pivot to root->left, reattach old left
		root->right = pivot;
		root->right->right = old_right;
		if (old_right)
			old_right->parent = root->right;

		// reassign pivot->right to root->right
		root->left = pivot->left;
		root->left->parent = root;

		// pivot's left child goes to root->left->right
		root->right->left = pivot_right;

		swap(&(root->right->value), &(root->value), sizeof(int));
		// pivot->left becomes root->right->max??
	}

	void rotation_RL(RedBlackNode *root)
	{
		// set the pivot, hang onto nodes that might get lost
		RedBlackNode *pivot = root->right;
		RedBlackNode *old_left = root->left;
		RedBlackNode *pivot_left_left = pivot->left->left;
		RedBlackNode *pivot_left_right = pivot->left->right;
	
		// the left of the pivot becomes the left of the root
		root->left = pivot->left;
		root->left->parent = root;
	
		// if the left of the pivot had a left child, it goes right of root->left
		root->left->right = pivot_left_left;
		if (root->left->right)
			root->left->right->parent = root->left;

		// if the left of the pivot had a right child, it goes left of the pivot
		pivot->left = pivot_left_right;
		if (pivot->left)
			pivot->left->parent = pivot;

		// restore old left of root
		root->left->left = old_left;
		if (old_left)
			old_left->parent = root->left;

		// swap values between the root and the left child
		swap(&(root->left->value), &root->value, sizeof(int));
	}

	void rotation_LR(RedBlackNode *root)
	{
		// set the pivot, hang onto nodes that might get lost
		RedBlackNode *pivot = root->left;
		RedBlackNode *old_right = root->right;
		RedBlackNode *pivot_right_right = pivot->right->right;
		RedBlackNode *pivot_right_left = pivot->right->left;
	
		// the right of the pivot becomes the right of the root
		root->right = pivot->right;
		root->right->parent = root;

		// if the right of the pivot had a right child, it goes left of root->right
		root->right->left = pivot_right_right;
		if (root->right->left)
			root->right->left->parent = root->right;

		// if the right of the pivot had a left child, it goes right of the pivot
		pivot->right = pivot_right_left;
		if (pivot->right)
			pivot->right->parent = pivot;

		// restore old left of root
		root->right->right = old_right;
		if (old_right)
			old_right->parent = root->right;

		// swap values between the root and the left child
		swap(&(root->right->value), &root->value, sizeof(int));
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
