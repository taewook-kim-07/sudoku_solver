#pragma once

#include "base.h"

void Sudoku_Count();

void Find_Candidate();

int Delete_Candidate(int x, int y, int except_number);

int Delete_RoworColinBox(int x, int y, int except_number, int type);
int Delete_RoworColoutBox(int x, int y, int except_number, int type);

int Compare_Candidate(int *candidate1, int *candidate2);

int Check_Row(int y, int real_number);
int Check_Column(int y, int real_number);
int Check_Box(int x, int y, int real_number);

int Naked_Single(int x, int y);
int Naked_Pair(int x, int y);

int Hidden_Single(int x, int y);
int Hidden_Pair(int x, int y);

int Locked_Candidates_Type1(int x, int y);
int Locked_Candidates_Type2(int x, int y);

int Backtracking_Solve(int x, int y);

int Sudoku_Solve(int (*argsudoku)[MAX]);