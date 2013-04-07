#pragma once
class RandomInt
{
private:
	int min;
	int max;

public:
	RandomInt(void);
	RandomInt(int min, int max);
	int get();
	int get(int min, int max);
	int getUnique();
	int getUnique(int min, int max);
	~RandomInt(void);
};

