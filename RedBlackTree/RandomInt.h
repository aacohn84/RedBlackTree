/*
	Author: Aaron Cohn

	A class for generating random positive integers.
*/

#pragma once

#include <vector>

class RandomInt
{
private:
	int min;
	int max;
	std::vector<int> *v;
	std::vector<int> *h;

public:
	RandomInt(int min, int max);
	int get();
	static int get(int min, int max);
	int getUnique();
	const std::vector<int>& history();
	~RandomInt(void);
};

