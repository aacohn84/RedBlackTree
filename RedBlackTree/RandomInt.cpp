#include "RandomInt.h"
#include <cstdlib>
#include <ctime>
#include <exception>

using namespace std;

RandomInt::RandomInt(void)
{
	srand((unsigned)time(NULL));
}


RandomInt::~RandomInt(void)
{
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
}

int RandomInt::get()
{
	return rand() % (max - min + 1) + min;
}

int RandomInt::get(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

int RandomInt::getUnique()
{
	return 0;
}

int RandomInt::getUnique(int min, int max)
{
	return 0;
}