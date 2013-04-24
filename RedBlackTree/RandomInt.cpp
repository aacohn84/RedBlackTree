#include "RandomInt.h"
#include <cstdlib>
#include <ctime>
#include <exception>
#include <algorithm>

using namespace std;

RandomInt::RandomInt(void)
{
	srand((unsigned)time(NULL));
	v = new vector<int>();
	h = new vector<int>();
}


RandomInt::~RandomInt(void)
{
	delete v;
}

RandomInt::RandomInt(int min, int max)
{
	srand((unsigned)time(NULL));
	if (min >= 0 && min < max)
	{
		this->min = min;
		this->max = max;
	}
	else
		throw std::exception("Range error: min must be >= 0 and min < max.");

	v = new vector<int>();
	h = new vector<int>();
	for (int i = 0; i < (max - min + 1); i++)
		v->push_back(i);
}

int RandomInt::get()
{
	return rand() % (max - min + 1) + min;
}

/* returns a random integer such that min <= n <= max */
int RandomInt::get(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

int RandomInt::getUnique()
{
	if (v->empty())
		throw std::exception("Error: range exhausted");

	// get a random number between 0 and the size of the range vector
	int i = get(0, v->size() - 1);	// 0 <= i < v->size
	auto it = v->begin() + i;

	// n will be the value currently at that position
	int n = *it;
	h->push_back(n);

	// erasing the element from the vector reduces the size of the vector and the pool of numbers to choose from
	v->erase(it);
	return n;
}

//int RandomInt::getUnique(int min, int max)
//{
//	int n;
//	vector<int>::iterator it;
//	do {
//		n = rand() % (max - min + 1) + min;
//		it = find(v->begin(), v->end(), n);
//	} while (it != v->end());
//	v->push_back(n);
//	return n;
//}

const std::vector<int>& RandomInt::history()
{
	return *h;
}
