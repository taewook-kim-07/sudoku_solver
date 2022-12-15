#include "base.h"

#define FileName "sudoku.txt"

int main()
{
	int sudoku[MAX][MAX]; // sudoku[x좌표(가로)][y좌표(세로)] 확정 숫자

	double timeSec = 0;
	LARGE_INTEGER start, finish, freq;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start); // CPU 클럭정보 가져오기
	
	if (!Sudoku_FileRead(FileName, sudoku)) // 스도쿠 파일 가져오기
	{
		printf("\"%s\" 읽기 실패\n", FileName);
		system("pause");
		return 0;
	}
	
	Sudoku_Print("풀이 시작", sudoku);

	if (Sudoku_Solve(sudoku)) // 스도쿠 풀이 시작 (성공 시 1)
		Sudoku_Print("풀이 완료", sudoku); // 스도쿠 9x9보여주기
	else
		printf("풀이 없음\n");
	
	Sudoku_Count();

	QueryPerformanceCounter(&finish); // CPU 클럭정보 가져오기
	timeSec = (finish.QuadPart - start.QuadPart) * 1000 / (freq.QuadPart);
	printf("소요 시간: %.fms\n", timeSec);

	system("pause");
	return 0;
}