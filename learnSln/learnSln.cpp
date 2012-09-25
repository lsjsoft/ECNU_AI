
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

	void get_infaline(unsigned int x, unsigned int y, tposV2& r)
	{
		tposV1 v;

		for(int ax=0; ax<QSIZE; ++ax)
		{
			if (ax!= x)
			{
				v.push_back(tpos(ax, y));
			}
		}
		r.push_back(v);
		v.clear();

		for(int ay=0; ay<QSIZE; ++ay)
		{
			if (ay!= y)
			{
				v.push_back(tpos(x, ay));
			}
		}
		r.push_back(v);
		v.clear();

		for(int ax=x+1, ay=y+1; ax<QSIZE && ax<QSIZE; ++ax, ++ay)
		{
			v.push_back( tpos(ax, ay));
		}
		if (!v.empty())
		{
			r.push_back(v);
			v.clear();
		}

		for(int ax=x+1, ay=y-1; ax<QSIZE && ay>=0; ++ax, --ay)
		{
			v.push_back( tpos(ax, ay));
		}
		if (!v.empty())
		{
			r.push_back(v);
			v.clear();
		}

		for(int ax=x-1, ay=y+1; ax>=0 && ax<QSIZE; --ax, ++ay)
		{
			v.push_back( tpos(ax, ay));
		}
		if (!v.empty())
		{
			r.push_back(v);
			v.clear();
		}

		for(int ax=x-1, ay=y-1; ax>=0 && ay>=0; --ax, --ay)
		{
			v.push_back( tpos(ax, ay));
		}
		if (!v.empty())
		{
			r.push_back(v);
			v.clear();
		}
	}

	bool can_put(unsigned int r, unsigned int c, const QTYPE& v)
	{
		TQueen<QTYPE, QSIZE> temp = *this;
		temp.set(r, c, v);


	}

	bool is_emtpy(const QTYPE& v)
	{
		return v == 0;
	}

	bool test_invalid() const
	{
		for( unsigned int y = 0; y< QSIZE; ++y)
		{
			unsigned int counter = 0;
			for( unsigned int x = 0; x < QSIZE; ++x)
			{
				const QTYPE& v= get(y, x);
				if (!is_emtpy(v))
				{
					++counter;
				}
			}

			if ( counter > 1)
			{
				return false;
			}
		}

		for( unsigned int x = 0; x< QSIZE; ++x)
		{
			unsigned int counter = 0;
			for( unsigned int y = 0; y < QSIZE; ++y)
			{
				const QTYPE& v= get(y, x);
				if (!is_emtpy(v))
				{
					++counter;
				}
			}

			if ( counter > 1)
			{
				return false;
			}
		}


		for()
	}

	const QTYPE& get(unsigned int r, unsigned int c)
	{
		return bound[r][c];
	}

	void set(const QTYPE& v, unsigned int r, unsigned int c)
	{
		bound[r][c]= v;
	}
};

TQueen<int, 8> obj;

int main(int argc, _TCHAR* argv[])
{
	tposV2 v;
	obj.get_infaline(0, 0, v);

	for each( const tposV1& p in v)
	{
		for each( const tpos& k in p)
		{
			printf("[%d,%d] ", k.x, k.y);
		}

		printf("\n");
	}

	return 0;
}
