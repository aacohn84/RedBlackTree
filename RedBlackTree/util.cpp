#include "util.h"
#include <stdlib.h> // malloc
#include <string.h> // memcpy

namespace util
{
	void swap(void *a, void *b, int size)
	{
		void *temp = malloc(size);
	
		memcpy(temp, a, size);
		memcpy(a, b, size);
		memcpy(b, temp, size);
	
		free(temp);
	}
};