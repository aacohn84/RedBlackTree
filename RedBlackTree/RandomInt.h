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
	RandomInt(void);
	RandomInt(int min, int max);
	int get();
	static int get(int min, int max);
	int getUnique();
	int getUnique(int min, int max);
	const std::vector<int>& history();
	~RandomInt(void);
};

