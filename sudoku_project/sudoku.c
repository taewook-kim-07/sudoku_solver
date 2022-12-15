#include "sudoku.h"

int (*sudoku)[MAX];
int candidate[MAX][MAX][MAX]; // candidate[x좌표(가로)][y좌표(세로)][(0~8까지 후보수 슬롯)] 후보수 저장 ( 후보수가 있다면 해당 슬롯에 1로 저장)

int countLogics = 0; // 스도쿠 알고리즘 작동 횟수
int countBrute  = 0;  // Backtracking 알고리즘 작동 횟수

void Sudoku_Count() 
{
	if (DEBUG || SHOWCOUNT) printf("알고리즘 %d회 / 순차대입 %d회\n", countLogics, countBrute); //debug
}

int Delete_Candidate(int x, int y, int except_number)
{
	int	minX = (x / BOX) * BOX, minY = (y / BOX) * BOX; // miX 3x3의 최소 좌표 구하기, minY 3x3의 최소 좌표 구하기
	for (int i = 0; i < 9; i++) // 0~8까지 반복
	{
		candidate[x][y][i] = 0; // x,y 좌표의 후보수 모두 삭제
		candidate[i][y][except_number] = 0; // y고정 / 모든 x좌표 해당 후보수 삭제
		candidate[x][i][except_number] = 0; // x고정 / 모든 y좌표 해당 후보수 삭제
		candidate[minX + i % BOX][minY + i / BOX][except_number] = 0; // x,y의 3x3 박스의 해당 후보수 삭제
	}
	return 1;
}

int Delete_RoworColinBox(int x, int y, int except_number, int type) // 박스 안의 행과 열의 후보수 삭제 (본인 제외)
{
	int	minX = (x / BOX) * BOX, minY = (y / BOX) * BOX;
	for (int i = 0; i < BOX; i++) // 박스 가로,세로 3만큼 동작
	{
		if (type == 0) // 본인 열 제외
		{
			if ((minX + i) != x) // 최소X + i가 x와 다를 경우 (해당 행의 열 후보수 전부 제거)
			{
				candidate[minX + i][minY + 0][except_number] = 0;
				candidate[minX + i][minY + 1][except_number] = 0;
				candidate[minX + i][minY + 2][except_number] = 0;
			}
		}
		else if (type == 1) //본인 행 제외
		{
			if ((minY + i) != y) // 최소Y + i가 y와 다를 경우 (해당 열의 행 후보수 전부 제거)
			{
				candidate[minX + 0][minY + i][except_number] = 0; // 왼쪽
				candidate[minX + 1][minY + i][except_number] = 0; // 중간
				candidate[minX + 2][minY + i][except_number] = 0; // 오른쪽
			}
		}
	}
	return 1;
}

int Delete_RoworColoutBox(int x, int y, int except_number, int type) // 박스 밖의 행과 열의 후보수 삭제
{
	int	minX = (x / BOX) * BOX, minY = (y / BOX) * BOX;
	for (int i = 0; i < MAX; i++) // 후보수 삭제
	{
		if (type == 0) // 박스 내 열 제외
		{
			if (minY != i && (minY + 1) != i && (minY + 2) != i) // 최소y(+1,+2)와 i가 다를 경우
			{
				candidate[x][i][except_number] = 0; // 후보수 해당 열의 행을 전부 제거
			}
		}
		else if (type == 1) // 박스 내 행 제외
		{
			if (minX != i && (minX + 1) != i && (minX + 2) != i) // 최소x(+1,+2)와 i가 다를 경우
			{
				candidate[i][y][except_number] = 0; // 후보수 해당 행의 열을 전부 제거
			}
		}
	}
	return 1;
}

int Compare_Candidate(int *candidate1, int *candidate2)
{
	for (int i = 0; i < MAX; i++) // 0~8 까지 반복
		if (candidate1[i] != candidate2[i]) return 0; // candidate1[i]와 candidate2[i]가 다르면 후보수가 일치하지 않아 return 0
	return 1; // 모든 후보수 Index 검사 후 일치하면 return 1
}

int Check_Row(int y, int real_number)
{
	for (int x = 0; x < MAX; x++)
	{
		if (sudoku[x][y] == real_number) // y고정, x가 바뀌면서 숫자가 있는지 확인 있으면 return 0
			return 0;
	}
	return 1;
}

int Check_Column(int x, int real_number)
{
	for (int y = 0; y < MAX; y++)
	{
		if (sudoku[x][y] == real_number) // x고정, y가 바뀌면서 숫자가 있는지 확인 있으면 return 0
			return 0;
	}
	return 1;
}

int Check_Box(int x, int y, int real_number)
{
	int minX = x / BOX * BOX,
		minY = y / BOX * BOX;
	for (int i = 0; i < MAX; i++)
		if (sudoku[minX + i % BOX][minY + i / BOX] == real_number) // minX, minY에서 0~2 더 해가면서 숫자가 있는지 확인 있으면 return 0
			return 0;
	return 1;
}

void Find_Candidate()
{
	int minX = 0, minY = 0, // [minX, minY] 3x3 박스의 좌상측 좌표 저장 변수
		tmpX = 0, tmpY = 0; // [tmpX, tmpY] 3x3 박스 내의 임시 좌표 저장 변수

	for (int y = 0; y < MAX; y++) // 0~8까지의 y좌표
	{
		for (int x = 0; x < MAX; x++) // 0~8까지의 x좌표
		{
			if (sudoku[x][y]) continue; // 스도쿠에 확정 숫자가 있다면 다음 좌표로 이동

			minX = (x / BOX) * BOX, minY = (y / BOX) * BOX; // miX 3x3의 최소 좌표 구하기, minY 3x3의 최소 좌표 구하기

			for (int i = 0; i < MAX; i++) // 후보수 0~8까지
				candidate[x][y][i] = 1;  // 후보수 Index 0~8 까지 1로 초기화

			for (int i = 0; i < MAX; i++) // X,Y 좌표 0~8까지
			{
				if (sudoku[i][y]) { candidate[x][y][sudoku[i][y] - 1] = 0; } // Y좌표 고정 / X좌표에 확정 수가 존재하면 x,y의 해당 숫자 후보수 제거
				if (sudoku[x][i]) { candidate[x][y][sudoku[x][i] - 1] = 0; } // X좌표 고정 / Y좌표에 확정 수가 존재하면 x,y의 해당 숫자 후보수 제거

				tmpX = minX + i % BOX, tmpY = minY + i / BOX; // tmpX는 i의 증가에 따라 0~2으로, tmpY는 i의 증가에 따라 0~2로 됨
				if ((x != tmpX && y != tmpY) && sudoku[tmpX][tmpY]) { candidate[x][y][sudoku[tmpX][tmpY] - 1] = 0; } // tmpX와 tmpY가 x,y 좌표와 다르고 확정 수가 있다면, x,y의 해당 숫자 후보수 제거
			}
		}
	}
}

int Naked_Single(int x, int y)
{
	int count = 0, tmp_num = -1; // count : 후보수 개수 확인, tmp_num : 후보수 임시 저장

	for (int i = 0; i < MAX; i++) // 0~8까지 반복
	{
		if (count > 1) break; // 후보수가 2개 이상이면 반복문 탈출
		if (candidate[x][y][i]) // 후보수가 존재한다면
		{
			tmp_num = i; // 후보수 Index 값을 tmp_num에 임시 저장
			count++; // 후보수 개수 카운팅 1 추가
		}
	}
	if (count == 1 && tmp_num != -1) // 후보수 개수가 1개고 tmp_num이 초기값이 아니라면
	{
		sudoku[x][y] = tmp_num + 1; // 스도쿠[x][y] 좌표에 tmp_num(후보수 Index) + 1하여 확정 수 정하기
		Delete_Candidate(x, y, tmp_num); // 후보수 제거 (본인, 행, 열, 3x3Box)

		if (DEBUG) printf("[X%d Y%d] Naked Single 확정숫자 %d\n", x + 1, y + 1, tmp_num + 1); // Debug
		countLogics++; // 스도쿠 알고리즘 작동 횟수 추가
		return 1; // Naked Single 작동됨 return 1
	}
	return 0; // Naked Single으로 처리 안됨 return 0
}

int Naked_Pair(int x, int y)
{
	int countRow = 0, countCol = 0, countBox = 0, // 행, 열, 박스의 후보수 중복 수 확인
		savedRow = 0, savedCol = 0, savedBox[2] = { 0,0 }, // 행, 열, 박스의 좌표 임시 저장
		tmp_candidate[2] = { 0 , 0 }; // 후보수 2개 임시 저장

	int minX = (x / BOX) * BOX, minY = (y / BOX) * BOX, // miX 3x3의 최소 좌표 구하기, minY 3x3의 최소 좌표 구하기
		tmpX = 0, tmpY = 0; // [tmpX, tmpY] 3x3 박스 내의 임시 좌표 저장 변수

	for (int i = 0; i < MAX; i++) // 0~8 반복 [후보수 개수 및 후보수 확인]
	{
		if (!candidate[x][y][i]) continue; // 후보수가 존재하지 않는 다면 다음으로 넘기기

		if (!tmp_candidate[0]) tmp_candidate[0] = i + 1; // tmp_candidate[0]이 0이라면 tmp_candidate[0]에 i + 1 저장(후보수)
		else if (!tmp_candidate[1]) tmp_candidate[1] = i + 1; // tmp_candidate[1]이 0이라면 tmp_candidate[1]에 i + 1 저장(후보수)
		else return 0; // tmp_candidate[0]과 tmp_candidate[1]에 후보수가 저장되어있으면 후보수가 3개이므로 Naked Pair 조건 충족 못함 return 0
	}

	if (tmp_candidate[0] && tmp_candidate[1]) // tmp_candidate[0]과 tmp_candidate[1]이 0이 아니라면 후보수가 2개여서 동작
	{
		tmp_candidate[0]--, tmp_candidate[1]--; // tmp_candidate[0]과 tmp_candidate[1]은 후보수여서 (-1하여) 후보수 Index로 만듬

		for (int i = 0; i < MAX; i++) // 0~8까지 반복
		{
			if (!sudoku[i][y] && x != i && Compare_Candidate(candidate[x][y], candidate[i][y])) countRow++, savedRow = i; // x와 i가 같지 않고(검사하는 좌표 제거), 후보수 x,y와 i,y가 같은 지 확인/ 같다면 countRow를 증가, savedRow에 해당 Row Index 저장
			if (!sudoku[x][i] && y != i && Compare_Candidate(candidate[x][y], candidate[x][i])) countCol++, savedCol = i; // y와 i가 같지 않고(검사하는 좌표 제거), 후보수 x,y와 x,i가 같은 지 확인/ 같다면 countCol를 증가, savedCol에 해당 Col Index 저장

			tmpX = minX + i % BOX, tmpY = minY + i / BOX; // tmpX와 tmpY의 임시 좌표 (0~2)
			if (!sudoku[tmpX][tmpY] && !(x == tmpX && y == tmpY) && Compare_Candidate(candidate[x][y], candidate[tmpX][tmpY])) countBox++, savedBox[0] = tmpX, savedBox[1] = tmpY; // x,y가 tmpX와 tmpY와 같지 않고 후보수 x,y와 tmpX, tmpY가 같으면 countBox 증가, savedBox[0]에 tmpx, savedBox[1]에 tmpY
		}

		if (countRow == 1) // X에서 후보수가 완전히 같은 곳이 1곳이라면
		{
			for (int i = 0; i < MAX; i++) { // 0~8 반복
				if (x == i || savedRow == i) continue; // x와 i가 같고, savedRow가 i와 같다면 continue (자기 좌표와 후보수가 일치하는 다른 좌표를 제외)

				candidate[i][y][tmp_candidate[0]] = 0; // 두 좌표를 제외하고 나머지의 후보수를 제거
				candidate[i][y][tmp_candidate[1]] = 0; // 두 좌표를 제외하고 나머지의 후보수를 제거
			}
		}

		if (countCol == 1)
		{
			for (int i = 0; i < MAX; i++) { // 0~8 반복
				if (y == i || savedCol == i) continue;  // y와 i가 같고, savedCol가 i와 같다면 continue (자기 좌표와 후보수가 일치하는 다른 좌표를 제외)

				candidate[x][i][tmp_candidate[0]] = 0; // 두 좌표를 제외하고 나머지의 후보수를 제거
				candidate[x][i][tmp_candidate[1]] = 0; // 두 좌표를 제외하고 나머지의 후보수를 제거
			}
		}

		if (countBox == 1)
		{
			for (int i = 0; i < MAX; i++) {
				tmpX = minX + i % BOX, tmpY = minY + i / BOX; // Box 안의 9개 좌표를 i의 증가에 따라 표현
				if ((x == tmpX && y == tmpY) || (savedBox[0] == tmpX && savedBox[1] == tmpY)) continue; // X,Y와 tmpX, tmpY가 같고, savedBox와 tmpX,Y가 같다면 제외

				candidate[tmpX][tmpY][tmp_candidate[0]] = 0; // 두 좌표를 제외하고 나머지의 후보수를 제거
				candidate[tmpX][tmpY][tmp_candidate[1]] = 0; // 두 좌표를 제외하고 나머지의 후보수를 제거
			}
		}

		if (countRow == 1 || countCol == 1 || countBox == 1) // 행, 열, 박스 중 자기 좌표를 제외한 후보수가 같은 것이 있다면
		{
			if (DEBUG) printf("[X%d Y%d] Naked Pair 후보숫자 %d, %d\n", x + 1, y + 1, tmp_candidate[0] + 1, tmp_candidate[1] + 1); // debug
			countLogics++; // 스도쿠 알고리즘 작동 횟수 추가
			return 1; // Naked Pair 작동 성공 return 1
		}
	}
	return 0; // Naked Pair 작동 실패 return 0
}

int Hidden_Single(int x, int y)
{
	int checkRow, checkCol, checkBox, // checkRow,Col,Box (행, 열, 박스에서 중복되지 않는 1개의 후보수를 체크하기 위해 임시기록)
		minX = (x / BOX) * BOX, minY = (y / BOX) * BOX,
		tmpX = 0, tmpY = 0;

	for (int number = 0; number < MAX; number++) // 0~8까지 (후보수)
	{
		if (!candidate[x][y][number]) continue; // 후보수[Index]가 존재하지 않는다면 다음 반복으로

		checkRow = 1, checkCol = 1, checkBox = 1; // 다음 number로 넘어가면 초기화

		for (int i = 0; i < MAX; i++) // 0~8까지
		{
			if (checkRow && x != i && candidate[i][y][number]) checkRow = 0; // checkRow가 1인 상태에서 본인을 제외하고 행에서 후보수가 같은 것이 있는 지 확인, 있다면 checkRow = 0
			if (checkCol && y != i && candidate[x][i][number]) checkCol = 0; // checkCol가 1인 상태에서 본인을 제외하고 열에서 후보수가 같은 것이 있는 지 확인, 있다면 checkCol = 0

			tmpX = minX + i % BOX, tmpY = minY + i / BOX;
			if (checkBox && (x != tmpX || y != tmpY) && candidate[tmpX][tmpY][number]) checkBox = 0; // checkBox가 1인 상태에서 본인 x,y좌표를 제외하고 중복된 후보수가 있는지 확인
		}
		if (checkRow || checkCol || checkBox) // 후보수가 중복된 것이 행, 열, 박스 중 하나라도 없다면
		{
			sudoku[x][y] = number + 1; // 해당 자리에 후보수 확정
			Delete_Candidate(x, y, number); // 행, 열, 박스에서 후보수 제거

			countLogics++; // 카운팅 증가
			if (DEBUG) printf("[X%d Y%d] Hidden Single 확정숫자 %d\n", x + 1, y + 1, number + 1);
			return 1; // Hidden Single 작동 성공 return 1
		}
	}
	return 0; // Hidden Single 작동 실패 return 0
}

int Hidden_Pair(int x, int y)
{
	int countRow = 0, countCol = 0, countBox = 0, // Row,Col,Box 같은 후보수 개수 확인
		tmpRow = 0, tmpCol = 0, tmpBox[2] = { 0, 0 }, // 후보수 저장
		savedBox[MAX][2], //  후보수 임시 좌표
		tmp_candidate[MAX], count_tmp = 0; // 후보수 임시저장, 후보수 개수 저장

	int	minX = (x / BOX) * BOX, minY = (y / BOX) * BOX,
		tmpX = 0, tmpY = 0;

	for (int i = 0; i < MAX; i++)
	{
		if (!candidate[x][y][i]) continue; // 후보수가 아니라면 제외
		tmp_candidate[count_tmp] = i; // 후보수 임시 저장
		count_tmp++; // 후보수 총 개수 증가
	}

	if (count_tmp > 2) // 후보수가 3개 이상이면
	{
		for (int i = 0; i < count_tmp; i++) // 후보수 총 개수 만큼 반복
		{
			countRow = 0, countCol = 0, countBox = 0, // 후보수 개수 확인 초기화
				tmpRow = 0, tmpCol = 0, tmpBox[0] = 0, tmpBox[1] = 0; // 후보수 임시 저장, 좌표 초기화

			for (int j = 0; j < MAX; j++) // 0~8까지 반복
			{
				if (x != j && candidate[j][y][tmp_candidate[i]]) tmpRow = j + 1, countRow++; // x와 j가 같지 않고 같은 후보수가 있다면, tmpRow에 위치저장, countRow에 찾은 개수 증가
				if (y != j && candidate[x][j][tmp_candidate[i]]) tmpCol = j + 1, countCol++; // y와 j가 같지 않고 같은 후보수가 있다면, tmpCol에 위치저장, countCol에 찾은 개수 증가

				tmpX = minX + j % BOX, tmpY = minY + j / BOX;
				if ((x != tmpX || y != tmpY) && candidate[tmpX][tmpY][tmp_candidate[i]]) tmpBox[0] = tmpX + 1, tmpBox[1] = tmpY + 1, countBox++;
			}

			if (countRow == 1) // 후보수가 행에서 중복된 것이 1개밖에 없다면
				savedBox[i][0] = tmpRow, savedBox[i][1] = y + 1; // 후보수 좌표 저장

			if (countCol == 1) // 후보수가 열에서 중복된 것이 1개밖에 없다면
				savedBox[i][0] = x + 1, savedBox[i][1] = tmpCol; // 후보수 좌표 저장

			if (countBox == 1) // 후보수가 박스에서 중복된 것이 1개밖에 없다면
				savedBox[i][0] = tmpBox[0], savedBox[i][1] = tmpBox[1]; // 후보수 좌표 저장

			if (!(countRow == 1 || countCol == 1 || countBox == 1)) // 행, 열, 박스에서 본인을 제외한 후보수의 위치가 1곳이 아니라면
				tmp_candidate[i] = -1; // 임시 후보수에서 제외
		}

		for (int i = 0; i < count_tmp; i++) // 후보수 총 개수 만큼 반복
		{
			if (tmp_candidate[i] == -1) continue; // 후보수가 -1이면 넘기기

			for (int j = i; j < count_tmp; j++) // i로 초기화해서 두 후보수 자리를 비교를 위해 반복문 사용
			{
				if (tmp_candidate[j] == -1 || i == j) continue; // i와 j가 같거나, 후보수가 -1이면 넘기기

				if (savedBox[i][0] == savedBox[j][0] && savedBox[i][1] == savedBox[j][1]) // savedBox[i]에는 첫번째 후보수 좌표, savedBox[j]에는 두번쨰 후보수 좌표가 서로 같다면 ([0]=x, [1]=y)
				{
					for (int k = 0; k < MAX; k++) // 후보수 제거를 위해 0~8
					{
						candidate[x][y][k] = 0; // 본인 좌표의 후보수를 모두 제거
						candidate[savedBox[i][0] - 1][savedBox[i][1] - 1][k] = 0; // 짝의 좌표의 후보수를 모두 제거
					}
					candidate[x][y][tmp_candidate[i]] = 1; // 본인 좌표에 후보수1을 표시
					candidate[x][y][tmp_candidate[j]] = 1; // 본인 좌표에 후보수2을 표시
					candidate[savedBox[i][0] - 1][savedBox[i][1] - 1][tmp_candidate[i]] = 1; // 짝의 좌표에 후보수1을 표시
					candidate[savedBox[i][0] - 1][savedBox[i][1] - 1][tmp_candidate[j]] = 1; // 짝의 좌표에 후보수2을 표시

					countLogics++; // 알고리즘 작동 횟수 증가
					if (DEBUG) printf("[X%d Y%d, X%d Y%d] Hidden Pair 후보숫자 %d, %d [i: %d ,j: %d]\n", x + 1, y + 1, savedBox[j][0], savedBox[j][1], tmp_candidate[i] + 1, tmp_candidate[j] + 1, i, j); // debug
					return 1; // Hidden Pair 작동 성공 return 1
				}
			}
		}
	}
	return 0; // Hidden Pair 작동 실패 return 0
}

int Locked_Candidates_Type1(int x, int y)
{
	int minX = (x / BOX) * BOX, minY = (y / BOX) * BOX,
		checkRow = 1, checkCol = 1, success = 0; // 박스 내 행, 열 중복 확인

	for (int number = 0; number < MAX; number++) // 0~8까지 반복
	{
		if (!candidate[x][y][number]) continue; // 후보수가 없다면 넘기기

		checkRow = 1, checkCol = 1; // 변수 초기화

		for (int i = 0; i < MAX; i++) // 0~8까지 반복
		{
			if (checkRow && (i != minX && i != minX + 1 && i != minX + 2) && candidate[i][y][number]) checkRow = 0; // minX(+1,+2) 박스 밖의 같은 행이 같은 후보수를 가지고 있는 지 확인되면 checkRow = 0
			if (checkCol && (i != minY && i != minY + 1 && i != minY + 2) && candidate[x][i][number]) checkCol = 0; // minY(+1,+2) 박스 밖의 같은 열이 같은 후보수를 가지고 있는 지 확인되면 checkCol = 0
		}

		if (checkRow) // 박스 밖의 행에 후보수가 없으면
		{
			Delete_RoworColinBox(x, y, number, 1); // 박스안의 본인 행을 제외하고 후보수 제거
			success = 1; // return 반환 값 (동작 성공)
			countLogics++; // 알고리즘 작동 횟수 증가
			if (DEBUG) printf("[X%d Y%d] L.C Type 1 Row 후보숫자 %d\n", x + 1, y + 1, number + 1); // debug
		}

		if (checkCol) // 박스 밖의 열에 후보수가 없으면
		{
			Delete_RoworColinBox(x, y, number, 0); // 박스안의 본인 열을 제외하고 후보수 제거
			success = 1; // reutrn 반환 값 (동작 성공)
			countLogics++; // 알고리즘 작동 횟수 증가
			if (DEBUG) printf("[X%d Y%d] L.C Type 1 Col 후보숫자 %d\n", x + 1, y + 1, number + 1); // debug
		}
	}
	return success; // success 값에 따라 return 다름
}

int Locked_Candidates_Type2(int x, int y)
{
	int minX = (x / BOX) * BOX, minY = (y / BOX) * BOX,
		tmpX = 0, tmpY = 0, // 
		checkRow = 1, checkCol = 1, success = 0;

	for (int number = 0; number < MAX; number++) // 0~8까지 반복 (후보수)
	{
		if (!candidate[x][y][number]) continue; // 후보수가 없으면 넘기기

		checkRow = 1, checkCol = 1; // 변수 초기화

		for (int i = 0; i < MAX; i++) // 0~8까지 반복 (x,y)
		{
			tmpX = minX + i % BOX, tmpY = minY + i / BOX;

			if (checkRow && y != tmpY && candidate[tmpX][tmpY][number]) checkRow = 0; // 박스 내 같은 행의 후보수가 있는 지 확인있으면 checkRow = 0
			if (checkCol && x != tmpX && candidate[tmpX][tmpY][number]) checkCol = 0; // 박스 내 같은 열의 후보수가 있는 지 확인있으면 checkCol = 0
		}

		if (checkRow) // 박스 내 행에서 같은 후보수가 없다면
		{
			Delete_RoworColoutBox(x, y, number, 1); // 박스 밖의 행 후보수를 제거
			success = 1; // return 반환 값 (동작 성공)
			countLogics++; // 알고리즘 작동 횟수 증가
			if (DEBUG) printf("[X%d Y%d] L.C Type 2 Row 후보숫자 %d\n", x + 1, y + 1, number + 1); // debug
		}

		if (checkCol) // 박스 내 열에서 같은 후보수가 없다면
		{
			Delete_RoworColoutBox(x, y, number, 0); // 박스 밖의 열 후보수를 제거
			success = 1; // return 반환 값 (동작 성공)
			countLogics++; // 알고리즘 작동 횟수 증가
			if (DEBUG) printf("[X%d Y%d] L.C Type 2 Col 후보숫자 %d\n", x + 1, y + 1, number + 1); // debug
		}
	}
	return success;
}

int Backtracking_Solve(int x, int y)
{
	if (sudoku[x][y]) // x,y에 확정 수가 있으면
	{
		if (x < 8) // x가 8보다 작으면
			return Backtracking_Solve(x + 1, y); // x를 1증가 후 재귀
		else if (y < 8) // y가 8보다 작으면
			return Backtracking_Solve(0, y + 1); // x를 0으로 초기화, y를 1증가 후 재귀
		else return 1; // 모든 x,y가 이상 없으므로 return 1
	}

	countBrute++; // 함수 호출 횟수 증가

	for (int i = 0; i < MAX; i++) // 대입 숫자 0~8 반복
	{
		if (candidate[x][y][i] && Check_Row(y, i + 1) && Check_Column(x, i + 1) && Check_Box(x, y, i + 1)) // 후보수가 있고, 후보수가 행, 열, 박스에서 중복된 확정수가 없다면
		{
			sudoku[x][y] = i + 1; // 스도쿠 확정 숫자 대입
			
			if (x < 8) // x가 8보다 작으면
				if (Backtracking_Solve(x + 1, y)) // x를 1증가 후 재귀
					return 1;
				else
					sudoku[x][y] = 0;
			else if (y < 8) // y가 8보다 작으면
				if (Backtracking_Solve(0, y + 1)) // x를 0으로 초기화, y를 1증가 후 재귀
					return 1;
				else
					sudoku[x][y] = 0;
			else return 1; // 모든 x,y가 이상 없으므로 return 1

		}
	}
	return 0; // 0~8반복에서 찾을 수 있는 방법이 없으면 return 0
}

int Sudoku_Solve(int (*argsudoku)[MAX])
{
	sudoku = argsudoku; // sudoku 포인터 지정

	Find_Candidate(); // 후보수 검색

	for (int y = 0; y < MAX; y++) // 0~8까지 반복 Y
	{
		for (int x = 0; x < MAX; x++) // 0~8까지 반복 X
		{
			if (sudoku[x][y]) continue; // 스도쿠 값 0이 아니라면 넘기기

			if (Naked_Single(x, y)) { y = -1; break; } // Nake Single 함수 호출, return 1이면 y=-1로 초기화 하고 x반복문 탈출 => x=0 y=0부터 다시 시작
			if (Hidden_Single(x, y)) { y = -1; break; } // Hidden Sigle 함수 호출, return 1이면 y=-1로 초기화 하고 x반복문 탈출 => x=0 y=0부터 다시 시작

			Naked_Pair(x, y); // Naked Pair 호출
			Hidden_Pair(x, y); // Hidden Pair 호출

			Locked_Candidates_Type1(x, y); // Locked Candidates Type1 호출
			Locked_Candidates_Type2(x, y); // Locked Candidates Type2 호출
		}
	}
	return Backtracking_Solve(0, 0);
}