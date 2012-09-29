
#include "stdafx.h"
#include <vector>

using namespace std;

struct tpos
{
	tpos(unsigned int ax, unsigned int ay): x(ax), y(ay) {}
	tpos(): x(0), y(0) {}

	unsigned int x;
	unsigned int y;
};

typedef vector<tpos> tposV1;
typedef vector<tposV1> tposV2;


template<typename QTYPE, unsigned int _QSIZE>
struct TQueen 
{
	enum { QSIZE=_QSIZE };

	QTYPE bound[QSIZE][QSIZE];

	TQueen()
	{
		memset(this, 0, sizeof(*this));
	}

	void get_infaline(unsigned int x, unsigned int y, tposV2& out) const
	{
		tposV1 line;

		for(int ax=0; ax<QSIZE; ++ax)
		{
			if (ax!= x)
			{
				line.push_back(tpos(ax, y));
			}
		}
		out.push_back(line);
		line.clear();

		for(int ay=0; ay<QSIZE; ++ay)
		{
			if (ay!= y)
			{
				line.push_back(tpos(x, ay));
			}
		}
		out.push_back(line);
		line.clear();

		for(int ax=x+1, ay=y+1; ax<QSIZE && ax<QSIZE; ++ax, ++ay)
		{
			line.push_back( tpos(ax, ay));
		}
		if (!line.empty())
		{
			out.push_back(line);
			line.clear();
		}

		for(int ax=x+1, ay=y-1; ax<QSIZE && ay>=0; ++ax, --ay)
		{
			line.push_back( tpos(ax, ay));
		}
		if (!line.empty())
		{
			out.push_back(line);
			line.clear();
		}

		for(int ax=x-1, ay=y+1; ax>=0 && ax<QSIZE; --ax, ++ay)
		{
			line.push_back( tpos(ax, ay));
		}
		if (!line.empty())
		{
			out.push_back(line);
			line.clear();
		}

		for(int ax=x-1, ay=y-1; ax>=0 && ay>=0; --ax, --ay)
		{
			line.push_back( tpos(ax, ay));
		}
		if (!line.empty())
		{
			out.push_back(line);
			line.clear();
		}
	}

	bool can_put(unsigned int y, unsigned int x, const QTYPE& v)
	{
		TQueen<QTYPE, QSIZE> temp = *this;
		temp.set(r, c, v);
	}

	bool is_emtpy(const QTYPE& v) const
	{
		return v == 0;
	}

	/** calc force */
	unsigned int counter_force(const tposV2& v2) const
	{
		unsigned int f= 0;
		for each( const tposV1& v1 in v2)
		{
			for each( const tpos& v in v1)
			{
				if (is_emtpy( get(v.y, v.x)))
				{
					++f;
				}
			}
		}
		return f;
	}

	bool is_valid() const
	{
		for(unsigned int y= 0; y< QSIZE; ++y)
		{
			for(unsigned int x=0; x< QSIZE; ++x)
			{
				if (!is_emtpy(get(y, x)))
				{
					tposV2 pv;
					get_infaline(x, y, pv);
					if (counter_force(pv) > 0)
					{
						return false;
					}
				}
			}
		}
		
		return true;
	}

	const QTYPE& get(unsigned int y, unsigned int x) const
	{
		return bound[y][x];
	}

	void set(unsigned int y, unsigned int x, const QTYPE& v)
	{
		bound[y][x]= v;
	}
};


int main(int argc, _TCHAR* argv[])
{
	TQueen<int, 8> obj;

	obj.set(0, 0, 1);
	const char* text1= obj.is_valid() ? "valid" : "invalid";

	obj.set(1, 1, 1);
	const char* text2= obj.is_valid() ? "valid" : "invalid";

	printf(text1);
	printf("\n");
	
	printf(text2);
	printf("\n");

	return 0;
}
