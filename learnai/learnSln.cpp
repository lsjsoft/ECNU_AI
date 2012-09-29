#include "stdafx.h"
#include "queen.h"
#include <windows.h>
#include "mmsystem.h"

#pragma comment(lib, "winmm.lib")

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

#define TEST_QUEEN_1(N) {	queen##N obj; queen##N##::queen_stack k= obj.layout_queen(); obj.view(); }
#define TEST_QUEEN_2(N) {	queen##N obj; queen##N##::queen_stack k= obj.layout_queen_v2(); obj.view(); }

#define TEST_QUEEN_SET(N) TEST_QUEEN_##N(4) TEST_QUEEN_##N(5) \
	TEST_QUEEN_##N(6) TEST_QUEEN_##N(7) TEST_QUEEN_##N(8) TEST_QUEEN_##N(9) TEST_QUEEN_##N(10)

int main(int argc, _TCHAR* argv[])
{
	typedef ntqueen<chess_piece, 4> queen4;
	typedef ntqueen<chess_piece, 5> queen5;
	typedef ntqueen<chess_piece, 6> queen6;
	typedef ntqueen<chess_piece, 7> queen7;
	typedef ntqueen<chess_piece, 8> queen8;
	typedef ntqueen<chess_piece, 9> queen9;
	typedef ntqueen<chess_piece, 10> queen10;

	unsigned long t1= timeGetTime();

	TEST_QUEEN_SET(1);

	unsigned long t2= timeGetTime();

	TEST_QUEEN_SET(2);

	unsigned long t3= timeGetTime();

	printf("%d, %d\n", t3-t2, t2-t1);

	return 0;
}
