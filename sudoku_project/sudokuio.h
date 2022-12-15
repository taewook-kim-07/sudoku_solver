#pragma once

#include "base.h"

int Sudoku_FileRead(char* path, int(*sudoku)[MAX]);
int Sudoku_Print(char *msg, int(*sudoku)[MAX]);