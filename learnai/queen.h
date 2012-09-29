#pragma once

enum { FORCE_DIR= 4 };

template<unsigned int SIZE>
struct ntpos_stack_uz
{
	ntpos_stack_uz() : usize(0) {}

	ntpos_ui pos[SIZE];
	unsigned int usize;

	void push(unsigned int ax, unsigned int ay)
	{
		pos[usize].x = ax;
		pos[usize].y = ay;
		++usize;
	}

	void pop()
	{
		if (usize> 0)
		{
			--usize;
		}
	}

	unsigned int size() const 
	{
		return usize;
	}
};

template<unsigned int SIZE>
struct flag_1d
{
	flag_1d(): size(0) {}
	ntpos_ui pos[SIZE];
	unsigned int size;

	void add(unsigned int x, unsigned int y)
	{
		pos[size].x = x;
		pos[size].y = y;
		++size;
	}
};

template<unsigned int SIZE>
struct flag_2d
{
	flag_1d<SIZE> lines[FORCE_DIR];

	void clear()
	{
		for(unsigned int i=0; i<FORCE_DIR; ++i)
		{
			lines[i].size = 0;
		}
	}
};

template<typename QUEEN_OBJECT, unsigned int A_BOUND_SIZE>
struct ntqueen 
{
	enum { BOUND_SIZE=A_BOUND_SIZE };

	QUEEN_OBJECT bound[BOUND_SIZE][BOUND_SIZE];

	typedef ntpos_stack_uz<BOUND_SIZE> queen_stack;
	typedef flag_2d<BOUND_SIZE> queen_flag_2d;
	typedef flag_1d<BOUND_SIZE> queen_flag_1d;

	ntqueen()
	{
		memset(this, 0, sizeof(*this));
	}

	bool layout_unique(queen_stack& tk)
	{
		size_t size = tk.size();
		for(unsigned int y=0; y<BOUND_SIZE; ++y)
		{
			for(unsigned int x=0; x<BOUND_SIZE; ++x)
			{
				if (! put_test(y, x))
				{
					continue;
				}

				tk.push(x, y);
				set(y, x, create_entity());

				if ( tk.size() == BOUND_SIZE)
				{
					return true;
				}

				if (!layout_unique(tk))
				{
					set(y, x, create_empty());
					tk.pop();
				}
			}
		}

		return tk.size() > size;
	}

	queen_stack layout_queen()
	{
		queen_stack r;
		layout_unique(r);
		return r;
	}

	void get_influence(unsigned int x, unsigned int y, queen_flag_2d& out) const
	{
		unsigned int idx= 0;

		out.clear();

		for(int ax=0; ax<BOUND_SIZE; ++ax)
		{
			if (ax != x)
			{
				out.lines[idx].add(ax, y);
			}
		}

		++idx;
		for(int ay=0; ay<BOUND_SIZE; ++ay)
		{
			if (ay!= y)
			{
				out.lines[idx].add(x, ay);
			}
		}

		++idx;
		for(int ax=x+1, ay=y+1; ax<BOUND_SIZE && ay<BOUND_SIZE; ++ax, ++ay)
		{
			out.lines[idx].add(ax, ay);
		}
		for(int ax=x+1, ay=y-1; ax<BOUND_SIZE && ay>=0; ++ax, --ay)
		{
			out.lines[idx].add(ax, ay);
		}

		++idx;
		for(int ax=x-1, ay=y+1; ax>=0 && ay<BOUND_SIZE; --ax, ++ay)
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

		ntqueen<QUEEN_OBJECT, BOUND_SIZE> obj(*this);
		obj.set(y, x, create_entity() );
		bool result= obj.is_valid();

		if (log_invalid && !result)
		{
			printf("invalid!\n");
			obj.view();
		}

		return result;
	}

	//////////////////////////////////////////////////////////////////////////

	bool is_emtpy_element(const QUEEN_OBJECT& v) const
	{
		return v.is_empty();
	}

	QUEEN_OBJECT create_entity() const
	{
		return QUEEN_OBJECT::create_entity();
	}

	QUEEN_OBJECT create_empty() const 
	{
		return QUEEN_OBJECT::create_empty();
	}

	//////////////////////////////////////////////////////////////////////////

	void print_element(const QUEEN_OBJECT& v)
	{
		printf("%d", v);
	}

	void _sprint_element(std::string& str, const QUEEN_OBJECT& v)
	{
		str += v.to_str();
	}

	bool is_empty_pos(unsigned int y, unsigned int x) const
	{
		return is_emtpy_element( get(y, x) );
	}

	unsigned int counter_force(queen_flag_2d& lineobj) const
	{
		unsigned int f= 0;
		for( unsigned int i=0; i<FORCE_DIR; ++i)
		{
			const queen_flag_1d& curr= lineobj.lines[i];

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
		queen_flag_2d out;
		for(unsigned int y= 0; y< BOUND_SIZE; ++y)
		{
			for(unsigned int x=0; x< BOUND_SIZE; ++x)
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

	const QUEEN_OBJECT& get(unsigned int y, unsigned int x) const
	{
		return bound[y][x];
	}

	void set(unsigned int y, unsigned int x, const QUEEN_OBJECT& v)
	{
		bound[y][x]= v;
	}

	void _sprint_head(std::string& str)
	{
		str += "   ";

		for(unsigned int i=0; i<BOUND_SIZE; ++i)
		{
			str+= ntbase::tostr(i);
			str+= " ";
		}

		str += "\n ©°";

		for(unsigned int i=0; i<BOUND_SIZE; ++i)
		{
			str+= "©¤";
		}

		str += "©´\n";
	}

	void _sprint_cont(std::string& str)
	{
		for(unsigned int y=0; y<BOUND_SIZE; ++y)
		{
			str += ntbase::tostr(y);
			str += "©¦";

			for(unsigned int x=0; x<BOUND_SIZE; ++x)
			{
				const QUEEN_OBJECT& element= get(y, x);
				_sprint_element(str, element);
				str += " ";
			}

			str += "©¦\n";
		}
	}

	void _sprint_end(std::string& str)
	{
		str += " ©¸";

		for(unsigned int i=0; i<BOUND_SIZE; ++i)
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
