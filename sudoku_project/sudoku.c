#include "sudoku.h"

int (*sudoku)[MAX];
int candidate[MAX][MAX][MAX]; // candidate[x��ǥ(����)][y��ǥ(����)][(0~8���� �ĺ��� ����)] �ĺ��� ���� ( �ĺ����� �ִٸ� �ش� ���Կ� 1�� ����)

int countLogics = 0; // ������ �˰��� �۵� Ƚ��
int countBrute  = 0;  // Backtracking �˰��� �۵� Ƚ��

void Sudoku_Count() 
{
	if (DEBUG || SHOWCOUNT) printf("�˰��� %dȸ / �������� %dȸ\n", countLogics, countBrute); //debug
}

int Delete_Candidate(int x, int y, int except_number)
{
	int	minX = (x / BOX) * BOX, minY = (y / BOX) * BOX; // miX 3x3�� �ּ� ��ǥ ���ϱ�, minY 3x3�� �ּ� ��ǥ ���ϱ�
	for (int i = 0; i < 9; i++) // 0~8���� �ݺ�
	{
		candidate[x][y][i] = 0; // x,y ��ǥ�� �ĺ��� ��� ����
		candidate[i][y][except_number] = 0; // y���� / ��� x��ǥ �ش� �ĺ��� ����
		candidate[x][i][except_number] = 0; // x���� / ��� y��ǥ �ش� �ĺ��� ����
		candidate[minX + i % BOX][minY + i / BOX][except_number] = 0; // x,y�� 3x3 �ڽ��� �ش� �ĺ��� ����
	}
	return 1;
}

int Delete_RoworColinBox(int x, int y, int except_number, int type) // �ڽ� ���� ��� ���� �ĺ��� ���� (���� ����)
{
	int	minX = (x / BOX) * BOX, minY = (y / BOX) * BOX;
	for (int i = 0; i < BOX; i++) // �ڽ� ����,���� 3��ŭ ����
	{
		if (type == 0) // ���� �� ����
		{
			if ((minX + i) != x) // �ּ�X + i�� x�� �ٸ� ��� (�ش� ���� �� �ĺ��� ���� ����)
			{
				candidate[minX + i][minY + 0][except_number] = 0;
				candidate[minX + i][minY + 1][except_number] = 0;
				candidate[minX + i][minY + 2][except_number] = 0;
			}
		}
		else if (type == 1) //���� �� ����
		{
			if ((minY + i) != y) // �ּ�Y + i�� y�� �ٸ� ��� (�ش� ���� �� �ĺ��� ���� ����)
			{
				candidate[minX + 0][minY + i][except_number] = 0; // ����
				candidate[minX + 1][minY + i][except_number] = 0; // �߰�
				candidate[minX + 2][minY + i][except_number] = 0; // ������
			}
		}
	}
	return 1;
}

int Delete_RoworColoutBox(int x, int y, int except_number, int type) // �ڽ� ���� ��� ���� �ĺ��� ����
{
	int	minX = (x / BOX) * BOX, minY = (y / BOX) * BOX;
	for (int i = 0; i < MAX; i++) // �ĺ��� ����
	{
		if (type == 0) // �ڽ� �� �� ����
		{
			if (minY != i && (minY + 1) != i && (minY + 2) != i) // �ּ�y(+1,+2)�� i�� �ٸ� ���
			{
				candidate[x][i][except_number] = 0; // �ĺ��� �ش� ���� ���� ���� ����
			}
		}
		else if (type == 1) // �ڽ� �� �� ����
		{
			if (minX != i && (minX + 1) != i && (minX + 2) != i) // �ּ�x(+1,+2)�� i�� �ٸ� ���
			{
				candidate[i][y][except_number] = 0; // �ĺ��� �ش� ���� ���� ���� ����
			}
		}
	}
	return 1;
}

int Compare_Candidate(int *candidate1, int *candidate2)
{
	for (int i = 0; i < MAX; i++) // 0~8 ���� �ݺ�
		if (candidate1[i] != candidate2[i]) return 0; // candidate1[i]�� candidate2[i]�� �ٸ��� �ĺ����� ��ġ���� �ʾ� return 0
	return 1; // ��� �ĺ��� Index �˻� �� ��ġ�ϸ� return 1
}

int Check_Row(int y, int real_number)
{
	for (int x = 0; x < MAX; x++)
	{
		if (sudoku[x][y] == real_number) // y����, x�� �ٲ�鼭 ���ڰ� �ִ��� Ȯ�� ������ return 0
			return 0;
	}
	return 1;
}

int Check_Column(int x, int real_number)
{
	for (int y = 0; y < MAX; y++)
	{
		if (sudoku[x][y] == real_number) // x����, y�� �ٲ�鼭 ���ڰ� �ִ��� Ȯ�� ������ return 0
			return 0;
	}
	return 1;
}

int Check_Box(int x, int y, int real_number)
{
	int minX = x / BOX * BOX,
		minY = y / BOX * BOX;
	for (int i = 0; i < MAX; i++)
		if (sudoku[minX + i % BOX][minY + i / BOX] == real_number) // minX, minY���� 0~2 �� �ذ��鼭 ���ڰ� �ִ��� Ȯ�� ������ return 0
			return 0;
	return 1;
}

void Find_Candidate()
{
	int minX = 0, minY = 0, // [minX, minY] 3x3 �ڽ��� �»��� ��ǥ ���� ����
		tmpX = 0, tmpY = 0; // [tmpX, tmpY] 3x3 �ڽ� ���� �ӽ� ��ǥ ���� ����

	for (int y = 0; y < MAX; y++) // 0~8������ y��ǥ
	{
		for (int x = 0; x < MAX; x++) // 0~8������ x��ǥ
		{
			if (sudoku[x][y]) continue; // ������ Ȯ�� ���ڰ� �ִٸ� ���� ��ǥ�� �̵�

			minX = (x / BOX) * BOX, minY = (y / BOX) * BOX; // miX 3x3�� �ּ� ��ǥ ���ϱ�, minY 3x3�� �ּ� ��ǥ ���ϱ�

			for (int i = 0; i < MAX; i++) // �ĺ��� 0~8����
				candidate[x][y][i] = 1;  // �ĺ��� Index 0~8 ���� 1�� �ʱ�ȭ

			for (int i = 0; i < MAX; i++) // X,Y ��ǥ 0~8����
			{
				if (sudoku[i][y]) { candidate[x][y][sudoku[i][y] - 1] = 0; } // Y��ǥ ���� / X��ǥ�� Ȯ�� ���� �����ϸ� x,y�� �ش� ���� �ĺ��� ����
				if (sudoku[x][i]) { candidate[x][y][sudoku[x][i] - 1] = 0; } // X��ǥ ���� / Y��ǥ�� Ȯ�� ���� �����ϸ� x,y�� �ش� ���� �ĺ��� ����

				tmpX = minX + i % BOX, tmpY = minY + i / BOX; // tmpX�� i�� ������ ���� 0~2����, tmpY�� i�� ������ ���� 0~2�� ��
				if ((x != tmpX && y != tmpY) && sudoku[tmpX][tmpY]) { candidate[x][y][sudoku[tmpX][tmpY] - 1] = 0; } // tmpX�� tmpY�� x,y ��ǥ�� �ٸ��� Ȯ�� ���� �ִٸ�, x,y�� �ش� ���� �ĺ��� ����
			}
		}
	}
}

int Naked_Single(int x, int y)
{
	int count = 0, tmp_num = -1; // count : �ĺ��� ���� Ȯ��, tmp_num : �ĺ��� �ӽ� ����

	for (int i = 0; i < MAX; i++) // 0~8���� �ݺ�
	{
		if (count > 1) break; // �ĺ����� 2�� �̻��̸� �ݺ��� Ż��
		if (candidate[x][y][i]) // �ĺ����� �����Ѵٸ�
		{
			tmp_num = i; // �ĺ��� Index ���� tmp_num�� �ӽ� ����
			count++; // �ĺ��� ���� ī���� 1 �߰�
		}
	}
	if (count == 1 && tmp_num != -1) // �ĺ��� ������ 1���� tmp_num�� �ʱⰪ�� �ƴ϶��
	{
		sudoku[x][y] = tmp_num + 1; // ������[x][y] ��ǥ�� tmp_num(�ĺ��� Index) + 1�Ͽ� Ȯ�� �� ���ϱ�
		Delete_Candidate(x, y, tmp_num); // �ĺ��� ���� (����, ��, ��, 3x3Box)

		if (DEBUG) printf("[X%d Y%d] Naked Single Ȯ������ %d\n", x + 1, y + 1, tmp_num + 1); // Debug
		countLogics++; // ������ �˰��� �۵� Ƚ�� �߰�
		return 1; // Naked Single �۵��� return 1
	}
	return 0; // Naked Single���� ó�� �ȵ� return 0
}

int Naked_Pair(int x, int y)
{
	int countRow = 0, countCol = 0, countBox = 0, // ��, ��, �ڽ��� �ĺ��� �ߺ� �� Ȯ��
		savedRow = 0, savedCol = 0, savedBox[2] = { 0,0 }, // ��, ��, �ڽ��� ��ǥ �ӽ� ����
		tmp_candidate[2] = { 0 , 0 }; // �ĺ��� 2�� �ӽ� ����

	int minX = (x / BOX) * BOX, minY = (y / BOX) * BOX, // miX 3x3�� �ּ� ��ǥ ���ϱ�, minY 3x3�� �ּ� ��ǥ ���ϱ�
		tmpX = 0, tmpY = 0; // [tmpX, tmpY] 3x3 �ڽ� ���� �ӽ� ��ǥ ���� ����

	for (int i = 0; i < MAX; i++) // 0~8 �ݺ� [�ĺ��� ���� �� �ĺ��� Ȯ��]
	{
		if (!candidate[x][y][i]) continue; // �ĺ����� �������� �ʴ� �ٸ� �������� �ѱ��

		if (!tmp_candidate[0]) tmp_candidate[0] = i + 1; // tmp_candidate[0]�� 0�̶�� tmp_candidate[0]�� i + 1 ����(�ĺ���)
		else if (!tmp_candidate[1]) tmp_candidate[1] = i + 1; // tmp_candidate[1]�� 0�̶�� tmp_candidate[1]�� i + 1 ����(�ĺ���)
		else return 0; // tmp_candidate[0]�� tmp_candidate[1]�� �ĺ����� ����Ǿ������� �ĺ����� 3���̹Ƿ� Naked Pair ���� ���� ���� return 0
	}

	if (tmp_candidate[0] && tmp_candidate[1]) // tmp_candidate[0]�� tmp_candidate[1]�� 0�� �ƴ϶�� �ĺ����� 2������ ����
	{
		tmp_candidate[0]--, tmp_candidate[1]--; // tmp_candidate[0]�� tmp_candidate[1]�� �ĺ������� (-1�Ͽ�) �ĺ��� Index�� ����

		for (int i = 0; i < MAX; i++) // 0~8���� �ݺ�
		{
			if (!sudoku[i][y] && x != i && Compare_Candidate(candidate[x][y], candidate[i][y])) countRow++, savedRow = i; // x�� i�� ���� �ʰ�(�˻��ϴ� ��ǥ ����), �ĺ��� x,y�� i,y�� ���� �� Ȯ��/ ���ٸ� countRow�� ����, savedRow�� �ش� Row Index ����
			if (!sudoku[x][i] && y != i && Compare_Candidate(candidate[x][y], candidate[x][i])) countCol++, savedCol = i; // y�� i�� ���� �ʰ�(�˻��ϴ� ��ǥ ����), �ĺ��� x,y�� x,i�� ���� �� Ȯ��/ ���ٸ� countCol�� ����, savedCol�� �ش� Col Index ����

			tmpX = minX + i % BOX, tmpY = minY + i / BOX; // tmpX�� tmpY�� �ӽ� ��ǥ (0~2)
			if (!sudoku[tmpX][tmpY] && !(x == tmpX && y == tmpY) && Compare_Candidate(candidate[x][y], candidate[tmpX][tmpY])) countBox++, savedBox[0] = tmpX, savedBox[1] = tmpY; // x,y�� tmpX�� tmpY�� ���� �ʰ� �ĺ��� x,y�� tmpX, tmpY�� ������ countBox ����, savedBox[0]�� tmpx, savedBox[1]�� tmpY
		}

		if (countRow == 1) // X���� �ĺ����� ������ ���� ���� 1���̶��
		{
			for (int i = 0; i < MAX; i++) { // 0~8 �ݺ�
				if (x == i || savedRow == i) continue; // x�� i�� ����, savedRow�� i�� ���ٸ� continue (�ڱ� ��ǥ�� �ĺ����� ��ġ�ϴ� �ٸ� ��ǥ�� ����)

				candidate[i][y][tmp_candidate[0]] = 0; // �� ��ǥ�� �����ϰ� �������� �ĺ����� ����
				candidate[i][y][tmp_candidate[1]] = 0; // �� ��ǥ�� �����ϰ� �������� �ĺ����� ����
			}
		}

		if (countCol == 1)
		{
			for (int i = 0; i < MAX; i++) { // 0~8 �ݺ�
				if (y == i || savedCol == i) continue;  // y�� i�� ����, savedCol�� i�� ���ٸ� continue (�ڱ� ��ǥ�� �ĺ����� ��ġ�ϴ� �ٸ� ��ǥ�� ����)

				candidate[x][i][tmp_candidate[0]] = 0; // �� ��ǥ�� �����ϰ� �������� �ĺ����� ����
				candidate[x][i][tmp_candidate[1]] = 0; // �� ��ǥ�� �����ϰ� �������� �ĺ����� ����
			}
		}

		if (countBox == 1)
		{
			for (int i = 0; i < MAX; i++) {
				tmpX = minX + i % BOX, tmpY = minY + i / BOX; // Box ���� 9�� ��ǥ�� i�� ������ ���� ǥ��
				if ((x == tmpX && y == tmpY) || (savedBox[0] == tmpX && savedBox[1] == tmpY)) continue; // X,Y�� tmpX, tmpY�� ����, savedBox�� tmpX,Y�� ���ٸ� ����

				candidate[tmpX][tmpY][tmp_candidate[0]] = 0; // �� ��ǥ�� �����ϰ� �������� �ĺ����� ����
				candidate[tmpX][tmpY][tmp_candidate[1]] = 0; // �� ��ǥ�� �����ϰ� �������� �ĺ����� ����
			}
		}

		if (countRow == 1 || countCol == 1 || countBox == 1) // ��, ��, �ڽ� �� �ڱ� ��ǥ�� ������ �ĺ����� ���� ���� �ִٸ�
		{
			if (DEBUG) printf("[X%d Y%d] Naked Pair �ĺ����� %d, %d\n", x + 1, y + 1, tmp_candidate[0] + 1, tmp_candidate[1] + 1); // debug
			countLogics++; // ������ �˰��� �۵� Ƚ�� �߰�
			return 1; // Naked Pair �۵� ���� return 1
		}
	}
	return 0; // Naked Pair �۵� ���� return 0
}

int Hidden_Single(int x, int y)
{
	int checkRow, checkCol, checkBox, // checkRow,Col,Box (��, ��, �ڽ����� �ߺ����� �ʴ� 1���� �ĺ����� üũ�ϱ� ���� �ӽñ��)
		minX = (x / BOX) * BOX, minY = (y / BOX) * BOX,
		tmpX = 0, tmpY = 0;

	for (int number = 0; number < MAX; number++) // 0~8���� (�ĺ���)
	{
		if (!candidate[x][y][number]) continue; // �ĺ���[Index]�� �������� �ʴ´ٸ� ���� �ݺ�����

		checkRow = 1, checkCol = 1, checkBox = 1; // ���� number�� �Ѿ�� �ʱ�ȭ

		for (int i = 0; i < MAX; i++) // 0~8����
		{
			if (checkRow && x != i && candidate[i][y][number]) checkRow = 0; // checkRow�� 1�� ���¿��� ������ �����ϰ� �࿡�� �ĺ����� ���� ���� �ִ� �� Ȯ��, �ִٸ� checkRow = 0
			if (checkCol && y != i && candidate[x][i][number]) checkCol = 0; // checkCol�� 1�� ���¿��� ������ �����ϰ� ������ �ĺ����� ���� ���� �ִ� �� Ȯ��, �ִٸ� checkCol = 0

			tmpX = minX + i % BOX, tmpY = minY + i / BOX;
			if (checkBox && (x != tmpX || y != tmpY) && candidate[tmpX][tmpY][number]) checkBox = 0; // checkBox�� 1�� ���¿��� ���� x,y��ǥ�� �����ϰ� �ߺ��� �ĺ����� �ִ��� Ȯ��
		}
		if (checkRow || checkCol || checkBox) // �ĺ����� �ߺ��� ���� ��, ��, �ڽ� �� �ϳ��� ���ٸ�
		{
			sudoku[x][y] = number + 1; // �ش� �ڸ��� �ĺ��� Ȯ��
			Delete_Candidate(x, y, number); // ��, ��, �ڽ����� �ĺ��� ����

			countLogics++; // ī���� ����
			if (DEBUG) printf("[X%d Y%d] Hidden Single Ȯ������ %d\n", x + 1, y + 1, number + 1);
			return 1; // Hidden Single �۵� ���� return 1
		}
	}
	return 0; // Hidden Single �۵� ���� return 0
}

int Hidden_Pair(int x, int y)
{
	int countRow = 0, countCol = 0, countBox = 0, // Row,Col,Box ���� �ĺ��� ���� Ȯ��
		tmpRow = 0, tmpCol = 0, tmpBox[2] = { 0, 0 }, // �ĺ��� ����
		savedBox[MAX][2], //  �ĺ��� �ӽ� ��ǥ
		tmp_candidate[MAX], count_tmp = 0; // �ĺ��� �ӽ�����, �ĺ��� ���� ����

	int	minX = (x / BOX) * BOX, minY = (y / BOX) * BOX,
		tmpX = 0, tmpY = 0;

	for (int i = 0; i < MAX; i++)
	{
		if (!candidate[x][y][i]) continue; // �ĺ����� �ƴ϶�� ����
		tmp_candidate[count_tmp] = i; // �ĺ��� �ӽ� ����
		count_tmp++; // �ĺ��� �� ���� ����
	}

	if (count_tmp > 2) // �ĺ����� 3�� �̻��̸�
	{
		for (int i = 0; i < count_tmp; i++) // �ĺ��� �� ���� ��ŭ �ݺ�
		{
			countRow = 0, countCol = 0, countBox = 0, // �ĺ��� ���� Ȯ�� �ʱ�ȭ
				tmpRow = 0, tmpCol = 0, tmpBox[0] = 0, tmpBox[1] = 0; // �ĺ��� �ӽ� ����, ��ǥ �ʱ�ȭ

			for (int j = 0; j < MAX; j++) // 0~8���� �ݺ�
			{
				if (x != j && candidate[j][y][tmp_candidate[i]]) tmpRow = j + 1, countRow++; // x�� j�� ���� �ʰ� ���� �ĺ����� �ִٸ�, tmpRow�� ��ġ����, countRow�� ã�� ���� ����
				if (y != j && candidate[x][j][tmp_candidate[i]]) tmpCol = j + 1, countCol++; // y�� j�� ���� �ʰ� ���� �ĺ����� �ִٸ�, tmpCol�� ��ġ����, countCol�� ã�� ���� ����

				tmpX = minX + j % BOX, tmpY = minY + j / BOX;
				if ((x != tmpX || y != tmpY) && candidate[tmpX][tmpY][tmp_candidate[i]]) tmpBox[0] = tmpX + 1, tmpBox[1] = tmpY + 1, countBox++;
			}

			if (countRow == 1) // �ĺ����� �࿡�� �ߺ��� ���� 1���ۿ� ���ٸ�
				savedBox[i][0] = tmpRow, savedBox[i][1] = y + 1; // �ĺ��� ��ǥ ����

			if (countCol == 1) // �ĺ����� ������ �ߺ��� ���� 1���ۿ� ���ٸ�
				savedBox[i][0] = x + 1, savedBox[i][1] = tmpCol; // �ĺ��� ��ǥ ����

			if (countBox == 1) // �ĺ����� �ڽ����� �ߺ��� ���� 1���ۿ� ���ٸ�
				savedBox[i][0] = tmpBox[0], savedBox[i][1] = tmpBox[1]; // �ĺ��� ��ǥ ����

			if (!(countRow == 1 || countCol == 1 || countBox == 1)) // ��, ��, �ڽ����� ������ ������ �ĺ����� ��ġ�� 1���� �ƴ϶��
				tmp_candidate[i] = -1; // �ӽ� �ĺ������� ����
		}

		for (int i = 0; i < count_tmp; i++) // �ĺ��� �� ���� ��ŭ �ݺ�
		{
			if (tmp_candidate[i] == -1) continue; // �ĺ����� -1�̸� �ѱ��

			for (int j = i; j < count_tmp; j++) // i�� �ʱ�ȭ�ؼ� �� �ĺ��� �ڸ��� �񱳸� ���� �ݺ��� ���
			{
				if (tmp_candidate[j] == -1 || i == j) continue; // i�� j�� ���ų�, �ĺ����� -1�̸� �ѱ��

				if (savedBox[i][0] == savedBox[j][0] && savedBox[i][1] == savedBox[j][1]) // savedBox[i]���� ù��° �ĺ��� ��ǥ, savedBox[j]���� �ι��� �ĺ��� ��ǥ�� ���� ���ٸ� ([0]=x, [1]=y)
				{
					for (int k = 0; k < MAX; k++) // �ĺ��� ���Ÿ� ���� 0~8
					{
						candidate[x][y][k] = 0; // ���� ��ǥ�� �ĺ����� ��� ����
						candidate[savedBox[i][0] - 1][savedBox[i][1] - 1][k] = 0; // ¦�� ��ǥ�� �ĺ����� ��� ����
					}
					candidate[x][y][tmp_candidate[i]] = 1; // ���� ��ǥ�� �ĺ���1�� ǥ��
					candidate[x][y][tmp_candidate[j]] = 1; // ���� ��ǥ�� �ĺ���2�� ǥ��
					candidate[savedBox[i][0] - 1][savedBox[i][1] - 1][tmp_candidate[i]] = 1; // ¦�� ��ǥ�� �ĺ���1�� ǥ��
					candidate[savedBox[i][0] - 1][savedBox[i][1] - 1][tmp_candidate[j]] = 1; // ¦�� ��ǥ�� �ĺ���2�� ǥ��

					countLogics++; // �˰��� �۵� Ƚ�� ����
					if (DEBUG) printf("[X%d Y%d, X%d Y%d] Hidden Pair �ĺ����� %d, %d [i: %d ,j: %d]\n", x + 1, y + 1, savedBox[j][0], savedBox[j][1], tmp_candidate[i] + 1, tmp_candidate[j] + 1, i, j); // debug
					return 1; // Hidden Pair �۵� ���� return 1
				}
			}
		}
	}
	return 0; // Hidden Pair �۵� ���� return 0
}

int Locked_Candidates_Type1(int x, int y)
{
	int minX = (x / BOX) * BOX, minY = (y / BOX) * BOX,
		checkRow = 1, checkCol = 1, success = 0; // �ڽ� �� ��, �� �ߺ� Ȯ��

	for (int number = 0; number < MAX; number++) // 0~8���� �ݺ�
	{
		if (!candidate[x][y][number]) continue; // �ĺ����� ���ٸ� �ѱ��

		checkRow = 1, checkCol = 1; // ���� �ʱ�ȭ

		for (int i = 0; i < MAX; i++) // 0~8���� �ݺ�
		{
			if (checkRow && (i != minX && i != minX + 1 && i != minX + 2) && candidate[i][y][number]) checkRow = 0; // minX(+1,+2) �ڽ� ���� ���� ���� ���� �ĺ����� ������ �ִ� �� Ȯ�εǸ� checkRow = 0
			if (checkCol && (i != minY && i != minY + 1 && i != minY + 2) && candidate[x][i][number]) checkCol = 0; // minY(+1,+2) �ڽ� ���� ���� ���� ���� �ĺ����� ������ �ִ� �� Ȯ�εǸ� checkCol = 0
		}

		if (checkRow) // �ڽ� ���� �࿡ �ĺ����� ������
		{
			Delete_RoworColinBox(x, y, number, 1); // �ڽ����� ���� ���� �����ϰ� �ĺ��� ����
			success = 1; // return ��ȯ �� (���� ����)
			countLogics++; // �˰��� �۵� Ƚ�� ����
			if (DEBUG) printf("[X%d Y%d] L.C Type 1 Row �ĺ����� %d\n", x + 1, y + 1, number + 1); // debug
		}

		if (checkCol) // �ڽ� ���� ���� �ĺ����� ������
		{
			Delete_RoworColinBox(x, y, number, 0); // �ڽ����� ���� ���� �����ϰ� �ĺ��� ����
			success = 1; // reutrn ��ȯ �� (���� ����)
			countLogics++; // �˰��� �۵� Ƚ�� ����
			if (DEBUG) printf("[X%d Y%d] L.C Type 1 Col �ĺ����� %d\n", x + 1, y + 1, number + 1); // debug
		}
	}
	return success; // success ���� ���� return �ٸ�
}

int Locked_Candidates_Type2(int x, int y)
{
	int minX = (x / BOX) * BOX, minY = (y / BOX) * BOX,
		tmpX = 0, tmpY = 0, // 
		checkRow = 1, checkCol = 1, success = 0;

	for (int number = 0; number < MAX; number++) // 0~8���� �ݺ� (�ĺ���)
	{
		if (!candidate[x][y][number]) continue; // �ĺ����� ������ �ѱ��

		checkRow = 1, checkCol = 1; // ���� �ʱ�ȭ

		for (int i = 0; i < MAX; i++) // 0~8���� �ݺ� (x,y)
		{
			tmpX = minX + i % BOX, tmpY = minY + i / BOX;

			if (checkRow && y != tmpY && candidate[tmpX][tmpY][number]) checkRow = 0; // �ڽ� �� ���� ���� �ĺ����� �ִ� �� Ȯ�������� checkRow = 0
			if (checkCol && x != tmpX && candidate[tmpX][tmpY][number]) checkCol = 0; // �ڽ� �� ���� ���� �ĺ����� �ִ� �� Ȯ�������� checkCol = 0
		}

		if (checkRow) // �ڽ� �� �࿡�� ���� �ĺ����� ���ٸ�
		{
			Delete_RoworColoutBox(x, y, number, 1); // �ڽ� ���� �� �ĺ����� ����
			success = 1; // return ��ȯ �� (���� ����)
			countLogics++; // �˰��� �۵� Ƚ�� ����
			if (DEBUG) printf("[X%d Y%d] L.C Type 2 Row �ĺ����� %d\n", x + 1, y + 1, number + 1); // debug
		}

		if (checkCol) // �ڽ� �� ������ ���� �ĺ����� ���ٸ�
		{
			Delete_RoworColoutBox(x, y, number, 0); // �ڽ� ���� �� �ĺ����� ����
			success = 1; // return ��ȯ �� (���� ����)
			countLogics++; // �˰��� �۵� Ƚ�� ����
			if (DEBUG) printf("[X%d Y%d] L.C Type 2 Col �ĺ����� %d\n", x + 1, y + 1, number + 1); // debug
		}
	}
	return success;
}

int Backtracking_Solve(int x, int y)
{
	if (sudoku[x][y]) // x,y�� Ȯ�� ���� ������
	{
		if (x < 8) // x�� 8���� ������
			return Backtracking_Solve(x + 1, y); // x�� 1���� �� ���
		else if (y < 8) // y�� 8���� ������
			return Backtracking_Solve(0, y + 1); // x�� 0���� �ʱ�ȭ, y�� 1���� �� ���
		else return 1; // ��� x,y�� �̻� �����Ƿ� return 1
	}

	countBrute++; // �Լ� ȣ�� Ƚ�� ����

	for (int i = 0; i < MAX; i++) // ���� ���� 0~8 �ݺ�
	{
		if (candidate[x][y][i] && Check_Row(y, i + 1) && Check_Column(x, i + 1) && Check_Box(x, y, i + 1)) // �ĺ����� �ְ�, �ĺ����� ��, ��, �ڽ����� �ߺ��� Ȯ������ ���ٸ�
		{
			sudoku[x][y] = i + 1; // ������ Ȯ�� ���� ����
			
			if (x < 8) // x�� 8���� ������
				if (Backtracking_Solve(x + 1, y)) // x�� 1���� �� ���
					return 1;
				else
					sudoku[x][y] = 0;
			else if (y < 8) // y�� 8���� ������
				if (Backtracking_Solve(0, y + 1)) // x�� 0���� �ʱ�ȭ, y�� 1���� �� ���
					return 1;
				else
					sudoku[x][y] = 0;
			else return 1; // ��� x,y�� �̻� �����Ƿ� return 1

		}
	}
	return 0; // 0~8�ݺ����� ã�� �� �ִ� ����� ������ return 0
}

int Sudoku_Solve(int (*argsudoku)[MAX])
{
	sudoku = argsudoku; // sudoku ������ ����

	Find_Candidate(); // �ĺ��� �˻�

	for (int y = 0; y < MAX; y++) // 0~8���� �ݺ� Y
	{
		for (int x = 0; x < MAX; x++) // 0~8���� �ݺ� X
		{
			if (sudoku[x][y]) continue; // ������ �� 0�� �ƴ϶�� �ѱ��

			if (Naked_Single(x, y)) { y = -1; break; } // Nake Single �Լ� ȣ��, return 1�̸� y=-1�� �ʱ�ȭ �ϰ� x�ݺ��� Ż�� => x=0 y=0���� �ٽ� ����
			if (Hidden_Single(x, y)) { y = -1; break; } // Hidden Sigle �Լ� ȣ��, return 1�̸� y=-1�� �ʱ�ȭ �ϰ� x�ݺ��� Ż�� => x=0 y=0���� �ٽ� ����

			Naked_Pair(x, y); // Naked Pair ȣ��
			Hidden_Pair(x, y); // Hidden Pair ȣ��

			Locked_Candidates_Type1(x, y); // Locked Candidates Type1 ȣ��
			Locked_Candidates_Type2(x, y); // Locked Candidates Type2 ȣ��
		}
	}
	return Backtracking_Solve(0, 0);
}