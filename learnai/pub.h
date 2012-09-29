#pragma once

#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <stack>
#include <string>

class ntbase
{
public:
	static const char* tostr(int i)
	{
		static char buff[32];
		sprintf_s(buff, "%d", i);
		return buff;
	}

	static const char* tostr(unsigned int i)
	{
		static char buff[32];
		sprintf_s(buff, "%d", i);
		return buff;
	}
};


template<typename T>
struct ntpos_t
{
	ntpos_t(T a, T b):x(a), y(b) {}
	ntpos_t():x(0), y(0) {}
	T x, y;
};

typedef ntpos_t<unsigned int> ntpos_ui;
typedef std::vector<ntpos_ui> ntpos_v1;
typedef std::vector<ntpos_v1> ntpos_v2;

