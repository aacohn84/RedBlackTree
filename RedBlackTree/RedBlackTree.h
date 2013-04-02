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
		RedBlackNode *left, *right, *parent;
		bool isRed, isLeaf;
		int blackHeight;

		RedBlackNode(Key key, Value value, bool external, RedBlackNode *parent)
			: isRed(external ? false : true), external(external), isLeaf(true),
			left(NULL), right(NULL), parent(parent)
		{
		}

		virtual ~RedBlackNode();
	};

public:
	RedBlackTree *root;

	RedBlackTree(void) : root(NULL) {}
	~RedBlackTree(void);
	
	void insert(Key key, Value value) 
	{
		if (!root)
		{
			root = new RedBlackNode(
		}
	}

	void remove(Key key) {}
	Value* find(Key key) {}
};
