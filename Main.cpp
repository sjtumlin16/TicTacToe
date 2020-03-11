//Including standard libraries
#include <iostream>
#include <cmath>
#include <iomanip>
#include <ctime> 
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
using namespace std;



int winTieLoss(int board[3][3]) {
	int check = 0;
	int temp = 0;

	for (int i = 0; i < 3; i++) {
		if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0]) {
			check = board[i][0];
		}
	}

	for (int i = 0; i < 3; i++) {
		if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i]) {
			check = board[i][0];
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < i + 1; j++) {
			temp = board[i][2];
			board[i][2] = board[i][1];
			board[i][1] = board[i][0];
			board[i][0] = temp;
		}
	}

	for (int i = 0; i < 3; i++) {
		if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i]) {
			check = board[i][0];
		}
	}

	return check;
}

int updateBoard(int row, int col, int player, int (&board)[3][3]) {
	board[row][col] = player;
	//printBoard(board);

	return 0;
}

int main()
{
	int board[3][3] = {{2, 2, 1}, {0, 2, 2}, {1, 2, 2}};
	cout << winTieLoss(board);

    return 0;
}