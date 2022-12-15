#include "sudokuio.h"

int Sudoku_FileRead(char* path, int (*sudoku)[MAX])
{
	FILE *file = fopen(path, "r"); // path 주소의 파일을 read한다는 것을 저장

	if (file == NULL) return 0;	// 파일 못 불러오면 0

	for (int y = 0; y < MAX; y++) // y축
		for (int x = 0; x < MAX; x++) // x축
			fscanf(file, "%d", &sudoku[x][y]); // sudoku[x][y]에 불러온 파일의 값을 하나 씩 저장

	fclose(file); // 파일 닫기
	return 1;
}

int Sudoku_Print(char *msg, int(*sudoku)[MAX])
{
	printf("-------------------------\n");
	for (int y = 0; y < MAX; y++)
	{
		printf("| %d %d %d | %d %d %d | %d %d %d |\n", sudoku[0][y], sudoku[1][y], sudoku[2][y], sudoku[3][y], sudoku[4][y], sudoku[5][y], sudoku[6][y], sudoku[7][y], sudoku[8][y]);
		if (!((y + 1) % 3)) // y가 2, 5, 8일때 작동 (2 + 1) % 3 = 0
			printf("-------------------------\n");
	}
	printf("%s\n", msg);
	return 1;
}