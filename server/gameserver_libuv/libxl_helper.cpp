#include <stdio.h>

#include "str.h"

#include "libxl_helper.h"
#include "def.h"

int libxl_helper::read_str_needfree(libxl::Sheet * &sheet, int row, int col, char* &ret){

	ASSERT(sheet && row < sheet->lastRow() && col < sheet->lastCol() && sheet->cellType(row, col) == CELLTYPE_STRING);

	const wchar_t* s = sheet->readStr(row, col);

	//char* ret = NULL;
	int len = w2c_needfree(s, ret);

	return len;
}