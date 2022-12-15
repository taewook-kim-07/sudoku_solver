#include "base.h"

#define FileName "sudoku.txt"

int main()
{
	int sudoku[MAX][MAX]; // sudoku[x��ǥ(����)][y��ǥ(����)] Ȯ�� ����

	double timeSec = 0;
	LARGE_INTEGER start, finish, freq;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start); // CPU Ŭ������ ��������
	
	if (!Sudoku_FileRead(FileName, sudoku)) // ������ ���� ��������
	{
		printf("\"%s\" �б� ����\n", FileName);
		system("pause");
		return 0;
	}
	
	Sudoku_Print("Ǯ�� ����", sudoku);

	if (Sudoku_Solve(sudoku)) // ������ Ǯ�� ���� (���� �� 1)
		Sudoku_Print("Ǯ�� �Ϸ�", sudoku); // ������ 9x9�����ֱ�
	else
		printf("Ǯ�� ����\n");
	
	Sudoku_Count();

	QueryPerformanceCounter(&finish); // CPU Ŭ������ ��������
	timeSec = (finish.QuadPart - start.QuadPart) * 1000 / (freq.QuadPart);
	printf("�ҿ� �ð�: %.fms\n", timeSec);

	system("pause");
	return 0;
}