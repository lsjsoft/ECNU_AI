#pragma once


enum { FORCE_DIR= 4 };

template<unsigned int SIZE>
struct ntpos_stack_uz
{
	ntpos_stack_uz() 
	: usize(0)
	, log_enable(false) 
	{

	}

	ntpos_ui pos[SIZE];
	unsigned int usize;

	void push(unsigned int ax, unsigned int ay)
	{
		pos[usize].x = ax;
		pos[usize].y = ay;
		++usize;

		if (log_enable)
		{
			for(unsigned int i=0; i<usize-1; ++i)
			{
				log_content+= "  ";
			}
			char buff[256];
			sprintf_s(buff, ">%d,%d\n", ax, ay);
			log_content += buff;
		}
	}

	void pop()
	{
		if (usize> 0)
		{
			--usize;

			if (log_enable)
			{
				for(unsigned int i=0; i<usize; ++i)
				{
					log_content+= "  ";
				}
				char buff[256];
				sprintf_s(buff, "<%d,%d\n", pos[usize].x, pos[usize].y);
				log_content += buff;
			}
		}
	}

	unsigned int size() const 
	{
		return usize;
	}

	bool log_enable;
	std::string log_content;
};

template<unsigned int D1SIZE>
struct flag_1d
{
	flag_1d(): size(0) {}
	ntpos_ui pos[D1SIZE];
	unsigned int size;

	void add(unsigned int x, unsigned int y)
	{
		pos[size].x = x;
		pos[size].y = y;
		++size;
	}
};

template<unsigned int D1SIZE, unsigned int D2SIZE>
struct flag_2d
{
	flag_1d<D1SIZE> lines[D2SIZE];

	enum { SIZE = D2SIZE };

	void clear()
	{
		for(unsigned int i=0; i<D2SIZE; ++i)
		{
			lines[i].size = 0;
		}
	}
};

template <unsigned int BOUND_SIZE>
struct influence_range
{
	influence_range()
	{
		memset(this, 0, sizeof(*this));
	}

	typedef flag_2d<BOUND_SIZE, FORCE_DIR> force_flag_2d;
	force_flag_2d inf_range[BOUND_SIZE][BOUND_SIZE];
};

template <unsigned int BOUND_SIZE>
struct force_range
{
	force_range()
	{
		memset(this, 0, sizeof(*this));
	}

	void add_force(unsigned int y, unsigned int x)
	{
		++force[y][x];
	}

	void dec_force(unsigned int y, unsigned int x)
	{
		--force[y][x];
	}

	int at(unsigned int y, unsigned int x) const 
	{
		return force[y][x];
	}

	int is_empty(unsigned int y, unsigned int x) const 
	{
		return at(y, x) == 0;
	}

	int force[BOUND_SIZE][BOUND_SIZE];
};

struct force_obj
{
	force_obj(unsigned int acod, unsigned int maxy): cod(acod), max_y(maxy) {}
	unsigned int cod;
	unsigned int max_y;
};

typedef std::vector<force_obj> force_objs;

inline bool operator < ( const force_obj& a, const force_obj& b)
{
	return a.max_y < b.max_y;
}

template<typename QUEEN_OBJECT, unsigned int A_BOUND_SIZE>
struct ntqueen 
{
	enum { BOUND_SIZE=A_BOUND_SIZE };
	typedef ntpos_stack_uz<BOUND_SIZE> queen_stack;
	typedef flag_2d<BOUND_SIZE, FORCE_DIR> force_flag_2d;
	typedef flag_1d<BOUND_SIZE> force_flag_1d;
	typedef force_range<BOUND_SIZE> queen_range;
	typedef influence_range<BOUND_SIZE> queen_inf_range;
	typedef QUEEN_OBJECT bound_t[BOUND_SIZE][BOUND_SIZE];

	bound_t bound;
	queen_inf_range influences;

	ntqueen()
	{
		memset(this, 0, sizeof(*this));
		_build_influences(influences);
	}

	void _build_inf_ele(unsigned int x, unsigned int y, force_flag_2d& out) const
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

	void _build_influences(queen_inf_range& infrange)
	{
		for(unsigned int y=0; y<BOUND_SIZE; ++y)
		{
			for(unsigned int x=0; x<BOUND_SIZE; ++x)
			{
				_build_inf_ele(x, y, infrange.inf_range[y][x]);
			}
		}
	}

	const force_flag_2d& get_influence(unsigned int y, unsigned int x) const
	{
		return influences.inf_range[y][x];
	}
	//////////////////////////////////////////////////////////////////////////

	bool layout_unique_v3(queen_stack& tk, queen_range& record, int sy=0)
	{
		size_t size = tk.size();
		for(unsigned int y=sy; y<BOUND_SIZE; ++y)
		{
			force_objs objs;
			objs.reserve(BOUND_SIZE);

			for(unsigned int x=0; x<BOUND_SIZE; ++x)
			{
				if (!record.is_empty(y, x))
				{
					continue;
				}

				if (! put_test(y, x))
				{
					continue;
				}

				const force_flag_2d& flag2d= get_influence(y, x);
				unsigned int num= get_max_line(flag2d);
				objs.push_back(force_obj(x, num));
			}

			if (objs.empty())
			{
				continue;
			}

			std::sort(objs.begin(), objs.end());

			for(unsigned int i=0; i<objs.size(); ++i)
			{
				unsigned int x = objs[i].cod;

				tk.push(x, y);
				set(y, x, create_entity());
				add_force_to_range(y, x, record);

				if ( tk.size() == BOUND_SIZE)
				{
					return true;
				}

				if (!layout_unique_v3(tk, record, sy+1))
				{
					set(y, x, create_empty());
					tk.pop();
					dec_force_to_range(y, x, record);
				}
				else
				{
					if ( tk.size() == BOUND_SIZE)
					{
						return true;
					}
				}
			}
		}

		return tk.size() > size;
	}

	queen_stack layout_queen_v3()
	{
		queen_stack r;
		//r.log_enable= true;
		queen_range record;
		layout_unique_v3(r, record);
		return r;
	}

	//////////////////////////////////////////////////////////////////////////

	void add_force_to_range(unsigned int y, unsigned int x, queen_range& record)
	{
		const force_flag_2d& out= get_influence(y, x);
		for(unsigned int i=0; i<out.SIZE; ++i)
		{
			for(unsigned int j=0; j<out.lines[i].size; ++j)
			{
				const ntpos_ui& pos= out.lines[i].pos[j];
				record.add_force(pos.y, pos.x);
			}
		}
		record.add_force(y, x);
	}

	void dec_force_to_range(unsigned int y, unsigned int x, queen_range& record)
	{
		const force_flag_2d& out= get_influence(y, x);
		for(unsigned int i=0; i<out.SIZE; ++i)
		{
			for(unsigned int j=0; j<out.lines[i].size; ++j)
			{
				const ntpos_ui& pos= out.lines[i].pos[j];
				record.dec_force(pos.y, pos.x);
			}
		}
		record.dec_force(y, x);
	}

	bool layout_unique_v2(queen_stack& tk, queen_range& record)
	{
		size_t size = tk.size();
		for(unsigned int y=0; y<BOUND_SIZE; ++y)
		{
			for(unsigned int x=0; x<BOUND_SIZE; ++x)
			{
				if (!record.is_empty(y, x))
				{
					continue;
				}

				if (! put_test(y, x))
				{
					continue;
				}

				tk.push(x, y);
				set(y, x, create_entity());
				add_force_to_range(y, x, record);

				if ( tk.size() == BOUND_SIZE)
				{
					return true;
				}

				if (!layout_unique_v2(tk, record))
				{
					set(y, x, create_empty());
					tk.pop();
					dec_force_to_range(y, x, record);
				}
				else
				{
					if ( tk.size() == BOUND_SIZE)
					{
						return true;
					}
				}
			}
		}

		return tk.size() > size;
	}

	queen_stack layout_queen_v2()
	{
		queen_stack r;
		queen_range record;
		layout_unique_v2(r, record);
		return r;
	}

	//////////////////////////////////////////////////////////////////////////

	bool layout_unique_v1(queen_stack& tk)
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

				if (!layout_unique_v1(tk))
				{
					set(y, x, create_empty());
					tk.pop();
				}
			}
		}

		return tk.size() > size;
	}

	queen_stack layout_queen_v1()
	{
		queen_stack r;
		layout_unique_v1(r);
		return r;
	}

	bool put_test(unsigned int y, unsigned int x) const
	{
		if (!is_empty_pos(y, x))
		{
			return false;
		}

		if (!is_valid())
		{
			return false;
		}

		ntqueen<QUEEN_OBJECT, BOUND_SIZE> obj(*this);
		obj.set(y, x, create_entity() );
		return obj.is_valid();
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

	unsigned int get_max_line(const force_flag_2d& lineobj) const
	{
		unsigned int max_y = 0;
		for( unsigned int i=2; i<FORCE_DIR; ++i)
		{
			const force_flag_1d& d1 = lineobj.lines[i];
			for(unsigned int j= 0; j<d1.size; ++j)
			{
				if (max_y < d1.pos[j].y)
				{
					max_y = d1.pos[j].y;
				}
			}
		}
		return max_y;
	}

	unsigned int counter_force(const force_flag_2d& lineobj) const
	{
		unsigned int f= 0;
		for( unsigned int i=0; i<FORCE_DIR; ++i)
		{
			const force_flag_1d& curr= lineobj.lines[i];

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
		for(unsigned int y= 0; y< BOUND_SIZE; ++y)
		{
			for(unsigned int x=0; x< BOUND_SIZE; ++x)
			{
				if (is_empty_pos(y, x))
				{
					continue;
				}

				const force_flag_2d& out= get_influence(y, x);
				if (counter_force(out) > 0)
				{
					return false;
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
		str += "    ";

		for(unsigned int i=0; i<BOUND_SIZE; ++i)
		{
			str+= ntbase::tostr(i);
			str+= " ";
		}

		str += "\n  ��";

		for(unsigned int i=0; i<BOUND_SIZE; ++i)
		{
			str+= "��";
		}

		str += "��\n";
	}

	void _sprint_cont(std::string& str)
	{
		for(unsigned int y=0; y<BOUND_SIZE; ++y)
		{
			str += ntbase::tostr2d(y);
			str += "��";

			for(unsigned int x=0; x<BOUND_SIZE; ++x)
			{
				const QUEEN_OBJECT& element= get(y, x);
				_sprint_element(str, element);
				str += " ";
			}

			str += "��\n";
		}
	}

	void _sprint_end(std::string& str)
	{
		str += "  ��";

		for(unsigned int i=0; i<BOUND_SIZE; ++i)
		{
			str+= "��";
		}

		str += "��\n";
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
