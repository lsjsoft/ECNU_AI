
#include "stdafx.h"
#include <vector>
#include <stack>

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
typedef std::stack<tpos> tposStk;

class Tbase
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

template<typename QTYPE, unsigned int _QSIZE>
struct TQueen 
{
	enum { QSIZE=_QSIZE, FORCE_DIR= 4, };

	QTYPE bound[QSIZE][QSIZE];

	struct flag_line
	{
		flag_line(): size(0) {}
		tpos pos[QSIZE];
		unsigned int size;

		void add(unsigned int x, unsigned int y)
		{
			pos[size].x = x;
			pos[size].y = y;
			++size;
		}
	};

	struct flag_lines
	{
		flag_line lines[FORCE_DIR];

		void clear()
		{
			for(unsigned int i=0; i<FORCE_DIR; ++i)
			{
				lines[i].size = 0;
			}
		}
	};

	struct queen_stack
	{
		queen_stack() : _size(0) {}

		tpos pos[QSIZE];
		unsigned int _size;

		void push(unsigned int ax, unsigned int ay)
		{
			pos[_size].x = ax;
			pos[_size].y = ay;
			++_size;
		}

		void pop()
		{
			if (_size> 0)
			{
				--_size;
			}
		}

		unsigned int size() const 
		{
			return _size;
		}
	};

	TQueen()
	{
		memset(this, 0, sizeof(*this));
	}

	bool layout_queen(queen_stack& tk)
	{
		size_t size = tk.size();
		for(unsigned int y=0; y<QSIZE; ++y)
		{
			for(unsigned int x=0; x<QSIZE; ++x)
			{
				if (! put_test(y, x))
				{
					continue;
				}

				tk.push(x, y);
				set(y, x, create_entity());

				if ( tk.size() == QSIZE)
				{
					return true;
				}

				if (!layout_queen(tk))
				{
					set(y, x, create_empty());
					tk.pop();
				}
			}
		}

		return tk.size() > size;
	}

	queen_stack layout_queenx()
	{
		queen_stack tk;
		layout_queen(tk);
		return tk;
	}

	void get_influence(unsigned int x, unsigned int y, flag_lines& out) const
	{
		unsigned int idx= 0;

		out.clear();

		for(int ax=0; ax<QSIZE; ++ax)
		{
			if (ax != x)
			{
				out.lines[idx].add(ax, y);
			}
		}

		++idx;
		for(int ay=0; ay<QSIZE; ++ay)
		{
			if (ay!= y)
			{
				out.lines[idx].add(x, ay);
			}
		}

		++idx;
		for(int ax=x+1, ay=y+1; ax<QSIZE && ay<QSIZE; ++ax, ++ay)
		{
			out.lines[idx].add(ax, ay);
		}
		for(int ax=x+1, ay=y-1; ax<QSIZE && ay>=0; ++ax, --ay)
		{
			out.lines[idx].add(ax, ay);
		}

		++idx;
		for(int ax=x-1, ay=y+1; ax>=0 && ay<QSIZE; --ax, ++ay)
		{
			out.lines[idx].add(ax, ay);
		}
		for(int ax=x-1, ay=y-1; ax>=0 && ay>=0; --ax, --ay)
		{
			out.lines[idx].add(ax, ay);
		}
	}

	bool put_test(unsigned int y, unsigned int x, bool log_invalid=false) const
	{
		if (!is_valid())
		{
			return false;
		}

		if (!is_empty_pos(y, x))
		{
			return false;
		}

		TQueen<QTYPE, QSIZE> obj(*this);
		obj.set(y, x, create_entity() );
		bool result= obj.is_valid();

		if (log_invalid && !result)
		{
			printf("invalid!\n");
			obj.view();
		}

		return result;
	}

	bool is_emtpy_element(const QTYPE& v) const
	{
		return v == QTYPE(0);
	}

	QTYPE create_entity() const
	{
		return QTYPE(1);
	}

	QTYPE create_empty() const 
	{
		return QTYPE(0);
	}

	void print_element(const QTYPE& v)
	{
		printf("%d", v);
	}

	void _sprint_element(std::string& str, const QTYPE& v)
	{
		str += Tbase::tostr(v);
	}

	bool is_empty_pos(unsigned int y, unsigned int x) const
	{
		return is_emtpy_element( get(y, x) );
	}

	unsigned int counter_force(const flag_lines& lineobj) const
	{
		unsigned int f= 0;
		for( unsigned int i=0; i<FORCE_DIR; ++i)
		{
			const flag_line& curr= lineobj.lines[i];

			for(unsigned int j=0; j<curr.size; ++j)
			{
				if (!is_empty_pos(curr.pos[j].y, curr.pos[j].x))
				{
					++f;
				}
			}
		}

		return f;
	}

	bool is_valid() const
	{
		flag_lines out;
		for(unsigned int y= 0; y< QSIZE; ++y)
		{
			for(unsigned int x=0; x< QSIZE; ++x)
			{
				if (!is_empty_pos(y, x))
				{
					get_influence(x, y, out);
					if (counter_force(out) > 0)
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

	void _sprint_head(std::string& str)
	{
		str += "   ";

		for(unsigned int i=0; i<QSIZE; ++i)
		{
			str+= Tbase::tostr(i);
			str+= " ";
		}

		str += "\n ©°";

		for(unsigned int i=0; i<QSIZE; ++i)
		{
			str+= "©¤";
		}

		str += "©´\n";
	}

	void _sprint_cont(std::string& str)
	{
 		for(unsigned int y=0; y<QSIZE; ++y)
 		{
			str += Tbase::tostr(y);
			str += "©¦";

 			for(unsigned int x=0; x<QSIZE; ++x)
 			{
 				const QTYPE& element= get(y, x);
				_sprint_element(str, element);
				str += " ";
 			}

 			str += "©¦\n";
 		}
	}

	void _sprint_end(std::string& str)
	{
		str += " ©¸";

		for(unsigned int i=0; i<QSIZE; ++i)
		{
			str+= "©¤";
		}

		str += "©¼\n";
	}

	void view()
	{
		std::string str;
		_sprint_head(str);
		_sprint_cont(str);
		_sprint_end(str);
		printf(str.c_str());
	}
};


int main(int argc, _TCHAR* argv[])
{
	typedef TQueen<int, 8> TQueen8;
	TQueen8 obj;
	TQueen8::queen_stack k= obj.layout_queenx();
	return 0;
}
