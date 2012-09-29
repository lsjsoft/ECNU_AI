#include "stdafx.h"
#include "queen.h"

int main(int argc, _TCHAR* argv[])
{
	typedef TQueen<int, 8> TQueen8;
	TQueen8 obj;
	ntpos_stack_uz<8> k= obj.layout_queenx();
	return 0;
}
