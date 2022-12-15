#include "sudokuio.h"

int Sudoku_FileRead(char* path, int (*sudoku)[MAX])
{
	FILE *file = fopen(path, "r"); // path �ּ��� ������ read�Ѵٴ� ���� ����

	if (file == NULL) return 0;	// ���� �� �ҷ����� 0

	for (int y = 0; y < MAX; y++) // y��
		for (int x = 0; x < MAX; x++) // x��
			fscanf(file, "%d", &sudoku[x][y]); // sudoku[x][y]�� �ҷ��� ������ ���� �ϳ� �� ����

	fclose(file); // ���� �ݱ�
	return 1;
}

int Sudoku_Print(char *msg, int(*sudoku)[MAX])
{
	printf("-------------------------\n");
	for (int y = 0; y < MAX; y++)
	{
		printf("| %d %d %d | %d %d %d | %d %d %d |\n", sudoku[0][y], sudoku[1][y], sudoku[2][y], sudoku[3][y], sudoku[4][y], sudoku[5][y], sudoku[6][y], sudoku[7][y], sudoku[8][y]);
		if (!((y + 1) % 3)) // y�� 2, 5, 8�϶� �۵� (2 + 1) % 3 = 0
			printf("-------------------------\n");
	}
	printf("%s\n", msg);
	return 1;
}