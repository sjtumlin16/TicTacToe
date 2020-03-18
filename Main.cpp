//Including standard libraries
#include <iostream>
#include <cmath>
#include <iomanip>
#include <ctime> 
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
using namespace std;

int compSmartPlay(int &compRow, int &compCol, int board[3][3]);
int winTieLoss(int board[3][3]);
bool playerSim(int compRow, int compCol, int board[3][3]);

void printBoard(int board[3][3])
{
    int rowNum = 3;
    int columnNum = 3;
    for(int n = 0; n < rowNum; n++) {
        for(int m = 0; m < columnNum; m++) {
            if(board[n][m] == 1) {
                cout << "O";
            }
            else if (board[n][m] == 2) {
                cout << "X";
            }
            else {
                cout << "-";
            }
        }
        cout << endl;
    }
    cout << endl;
}

int updateBoard(int row, int col, int player, int (&board)[3][3]) 
{
	board[row][col] = player; 
	printBoard(board);

	return winTieLoss(board);
}

void playerInput(int &playerRow, int &playerColumn, int board[3][3])
{
    do {
        cout << "What Row (0,1, or 2) would you like to place your 'O' in?" << endl;
        cin >> playerRow;

        do {
            cout << "What Column (0,1, or 2) would you like to place your 'O' in?" << endl;
            cin >> playerColumn;
        } while(playerColumn < 0 || playerColumn > 2);

    } while(playerRow < 0 || playerRow > 2 || board[playerRow][playerColumn]); //checks if it is not a "O" already exsists in the array

    cout << endl;
}

void incCoords(int& row, int& col) 
{
	if (col == 2) {
		row++;
		col = 0;
	}
	else {
		col++;
	}
}

void compDumbPlay(int &compRow, int &compCol, int board[3][3])
{
    do {
        compRow = rand() % 3; 

        do {
        	compCol = rand() % 3; 
        } while(compCol < 0 || compCol > 2);

    } while(compRow < 0 || compRow > 2 || board[compRow][compCol]); //checks if it is not a "O" already exsists in the array
}

int winTieLoss(int board[3][3]) 
{
	int check = 0;
	int temp = 0;
	int count = 0;
	int boardCopy[3][3] = {board[3][3]};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			boardCopy[i][j] = board[i][j];
			if (boardCopy[i][j] != 0) {
				count++;
			}
		}
	}

	for (int i = 0; i < 3; i++) {
		if (boardCopy[i][0] == boardCopy[i][1] && boardCopy[i][1] == boardCopy[i][2] && boardCopy[i][0]) {
			check = boardCopy[i][0];
		}
	}

	for (int i = 0; i < 3; i++) {
		if (boardCopy[0][i] == boardCopy[1][i] && boardCopy[1][i] == boardCopy[2][i] && boardCopy[0][i]) {
			check = boardCopy[0][i];
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < i + 1; j++) {
			temp = boardCopy[i][2];
			boardCopy[i][2] = boardCopy[i][1];
			boardCopy[i][1] = boardCopy[i][0];
			boardCopy[i][0] = temp;
		}
	}

	if (boardCopy[0][0] == boardCopy[1][0] && boardCopy[1][0] == boardCopy[2][0] && boardCopy[0][0]) {
		check = boardCopy[0][0];
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2 * i + 1; j++) {
			temp = boardCopy[i][0];
			boardCopy[i][0] = boardCopy[i][1];
			boardCopy[i][1] = boardCopy[i][2];
			boardCopy[i][2] = temp;
		}
	}

	if (boardCopy[0][0] == boardCopy[1][0] && boardCopy[1][0] == boardCopy[2][0] && boardCopy[0][0]) {
		check = boardCopy[0][0];
	}

	if (check == 0 && count == 9) {
		check = 3;
	}

	return check;
}

int playerSmartPlay(int &compRow, int &compCol, int board[3][3])
{
	int boardCopy[3][3] = {board[3][3]};
	int check;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			boardCopy[i][j] = board[i][j];
		}
	}

	do {
	    do {
	        compRow = rand() % 3;

	        do {
	        	compCol = rand() % 3; 
	        } while (compCol < 0 || compCol > 2);

	    } while (compRow < 0 || compRow > 2 || boardCopy[compRow][compCol]);
		check = playerSim(compRow, compCol, boardCopy);
	} while (check);

	return check;
}

bool playerSim(int playerRow, int playerCol, int board[3][3])
{
	int boardCopy[3][3] = {board[3][3]};
	int compRow, compCol;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			boardCopy[i][j] = board[i][j];
		}
	}

	cout << "playerSim/playerInit" << endl;
    if (updateBoard(playerRow, playerCol, 1, boardCopy)) {
    	return (winTieLoss(boardCopy) != 1);
    }
/*
	compSmartPlay(compRow, compCol, boardCopy);
	cout << "playerSim/comp" << endl;
	if (updateBoard(compRow, compCol, 2, boardCopy)) {
		return (winTieLoss(boardCopy) != 1);
	}*/

	return true;
}

/* /////////This sort of works but I'm testing out a new one that isn't recursive bc apperently I don't know how to do that.///////
bool compSim(int compRow, int compCol, int board[3][3])
{
	int boardCopy[3][3] = {board[3][3]};
	int playerRow, playerCol;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			boardCopy[i][j] = board[i][j];
		}
	}

	cout << "compSim/compInit" << endl;
    if (updateBoard(compRow, compCol, 2, boardCopy)) {
    	return (winTieLoss(boardCopy) != 1);
    }*/
/*
	playerSmartPlay(playerRow, playerCol, boardCopy);
	cout << "compSim/player" << endl;
	if (updateBoard(playerRow, playerCol, 1, boardCopy)) {
		return (winTieLoss(boardCopy) != 1);
	}*//*

	return true;
}
*/
bool compSim(int compRow, int compCol, int board[3][3])
{
	int boardCopy[3][3] = {board[3][3]};
	int playerRow, playerCol;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			boardCopy[i][j] = board[i][j];
		}
	}

	cout << "init update" << endl;
	updateBoard(compRow, compCol, 2, boardCopy);

	for (int k = 0; k < 3; k++) {
		do {
		    do {
		        playerRow = rand() % 3;

		        do {
		        	playerCol = rand() % 3; 
		        } while (playerCol < 0 || playerCol > 2);

		    } while (playerRow < 0 || playerRow > 2 || boardCopy[playerRow][playerCol]);
			cout << "player update" << endl;
		    updateBoard(playerRow, playerCol, 1, boardCopy);

			if (winTieLoss(boardCopy)) {
				if (winTieLoss(boardCopy) != 1) {
					return true;
				}
				else {
					cout << "going" << endl;
					goto label;
				}
			}

		    do {
		        compRow = rand() % 3;

		        do {
		        	compCol = rand() % 3; 
		        } while (compCol < 0 || compCol > 2);

		    } while (compRow < 0 || compRow > 2 || boardCopy[compRow][compCol]);
			cout << "comp update" << endl;
		    updateBoard(compRow, compCol, 2, boardCopy);

			if (winTieLoss(boardCopy)) {
				if (winTieLoss(boardCopy) != 1) {
					return true;
				}
				else {
					cout << "going" << endl;
					goto label;
				}
			}

		} while (!winTieLoss(boardCopy));

		label:
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				boardCopy[i][j] = board[i][j];
			}
		}

	} 

	return true;
}

bool playerCheck(int &compRow, int &compCol, int board[3][3]) {
	int boardCopy[3][3] = {board[3][3]};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			boardCopy[i][j] = board[i][j];
		}
	}

	compRow = 0;
	compCol = 0;

	for (int i = 0; i < 9; i++) {
		if (!boardCopy[compRow][compCol]) {
			updateBoard(compRow, compCol, 1, boardCopy);
		}

		if (winTieLoss(boardCopy)) {
			return true;
		}

		incCoords(compRow, compCol);

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				boardCopy[i][j] = board[i][j];
			}
		}		
	}

	return false;
}

int compSmartPlay(int &compRow, int &compCol, int board[3][3])
{
	int boardCopy[3][3] = {board[3][3]};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			boardCopy[i][j] = board[i][j];
		}
	}

	if (playerCheck(compRow, compCol, boardCopy)) {
		return true;
	}

	do {
	    do {
	        compRow = rand() % 3;

	        do {
	        	compCol = rand() % 3; 
	        } while (compCol < 0 || compCol > 2);

	    } while (compRow < 0 || compRow > 2 || boardCopy[compRow][compCol]);
	    cout << "Running CompSim" << endl;
	} while (!compSim(compRow, compCol, boardCopy));

	return true;
}

int main()
{	
	char token;

	do {
		srand(time(NULL));

		int board[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
		int playerRow, playerCol, compRow, compCol, round = 0;

		cout << "Computer goes first:" << endl << endl;

		while (true) {

			if (round) {
				compSmartPlay(compRow, compCol, board);
			}
			else {
				compDumbPlay(compRow, compCol, board);			
			}
			if (updateBoard(compRow, compCol, 2, board)) {
				break;
			}

			playerInput(playerRow, playerCol, board);
			if (updateBoard(playerRow, playerCol, 1, board)) {
				break;
			}

			round++;
		}

		switch (winTieLoss(board)) {
			case 1: 
				cout << "Congrats, you won!" << endl;
			break;
			case 2:
				cout << "COMPUTERS ARE SUPERIOR" << endl;
			break;
			case 3:
				cout << "Dang! It was a tie!" << endl;
			break;
			default:
				cout << "FATAL ERROR" << endl;
		}

		cout << "So... Do you want to play again?" << endl;
		do {
			cout << "'Y' or 'N'" << endl;
			cin >> token;
			
		} while (token != 'Y' && token != 'N');
		cout << endl;

	} while(token == 'Y');

    return 0;
}
