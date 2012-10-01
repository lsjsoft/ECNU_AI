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


void sum2(int n, int* __restrict a, int* __restrict b, int * c, int * d) 
{
	int i;
	for (i = 0; i < n; i++) 
	{
		a[i] = b[i] + c[i];
		c[i] = b[i] + d[i];
	}
}

int main(int argc, _TCHAR* argv[])
{

	unsigned long t2= timeGetTime();

	typedef ntqueen<chess_piece, 8> queen11;
	queen11 obj; 
	queen11::queen_stack k= obj.layout_queen_v3(); 
	obj.view();

	unsigned long t3= timeGetTime();

	printf(k.log_content.c_str());

	printf("%d\n", t3-t2);


	return 0;
}
