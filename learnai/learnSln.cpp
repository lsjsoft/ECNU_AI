#include "stdafx.h"
#include "queen.h"

struct chess_piece
{
	enum 
	{
		EMPTY = 0,
		ENTITY= 1,
	};

	int piece_data;

	chess_piece(): piece_data(EMPTY) {}
	chess_piece(int d): piece_data(d) {}

	bool is_empty() const { return piece_data== EMPTY; }

	bool operator == ( const chess_piece& obj) 
	{
		return obj.piece_data == piece_data;
	}

	const char* to_str() const
	{
		return ntbase::tostr(piece_data);
	}

	static chess_piece create_empty() 
	{ 
		return chess_piece(EMPTY);
	}

	static chess_piece create_entity()
	{
		return chess_piece(ENTITY);
	}
};

int main(int argc, _TCHAR* argv[])
{
	typedef ntqueen<chess_piece, 4> queen4;
	typedef ntqueen<chess_piece, 5> queen5;
	typedef ntqueen<chess_piece, 6> queen6;
	typedef ntqueen<chess_piece, 7> queen7;

	{
		queen4 obj;
		queen4::queen_stack k= obj.layout_queen();
		obj.view();
	}

	{
		queen5 obj;
		queen5::queen_stack k= obj.layout_queen();
		obj.view();
	}

	{
		queen6 obj;
		queen6::queen_stack k= obj.layout_queen();
		obj.view();
	}

	{
		queen7 obj;
		queen7::queen_stack k= obj.layout_queen();
		obj.view();
	}


	return 0;
}
