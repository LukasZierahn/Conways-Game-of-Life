#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <time.h>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>

#ifndef IncludeHeader
#define IncludeHeader

class Node;

using namespace std;

struct Cell
{
	int x = 0, y = 0;
};

struct Token
{
	int x = 0, y = 0;
	int aliveNextToMe = 0;
	bool aliveOnMe = false;
};

inline char ConvertIntToAlphabetic(int i)
{
	if (i < 10)
	{
		return i + 48;
	}
	else if (i < 37)
	{
		return i + 65;
	}

	return NULL;
}

#endif