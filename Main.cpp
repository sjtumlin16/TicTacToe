//Including necessary libraries
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

// Need prototypes for these funcs because of the way the fucncs call each other,
// it wouldn't be possible to not
int compSmartPlay(int &compRow, int &compCol, int board[3][3]);
int winTieLoss(int board[3][3]);

// This func is used to print the board to let the player see it
// Takes the board array, returns nothing 
void printBoard(int board[3][3])
{
	// Variables to show how many rows/columns we have
    int rowNum = 3;
    int columnNum = 3;

    // Print header for the table
    cout << "~|ABC~" << endl;
    // Increment to print rows
    for(int n = 0; n < rowNum; n++) {
    	// Print the left column with row labels (one more than the index)
    	// then a | to divide it from the body
    	cout << n + 1 << "|";
    	// Increment to print the body columns
        for(int m = 0; m < columnNum; m++) {
        	// If our board has a 1 in that spot, it's a player so print O
            if(board[n][m] == 1) {
                cout << "O";
            }
            // If its a 2, that's the computer, so print an X
            else if (board[n][m] == 2) {
                cout << "X";
            }
            // If you got here then theres nothing, so print a -
            else {
                cout << "-";
            }
        }
        // End of row
        cout << endl;
    }
    // End of table, extra line for some breathing room
    cout << endl;
}

// Takes the row and column to be updated, the player that wants to update,
// the board (as reference because you can't return arrays), and whether or not the 
// new board should be printed (you don't want to print it if it's called by compSim)
// Returns 0 for success, 1 for player win, 2 for comp win, and 3 for a tie
int updateBoard(int row, int col, int player, int (&board)[3][3], bool print) 
{
	// Update the requested spot with the player
	// We don't check if it's valid; that's the other funcs job
	board[row][col] = player; 

	// If requested, print the updated board
	if (print) {
		printBoard(board);
	}

	// Check if there's any terminal result and return the result
	return winTieLoss(board);
}

// Takes row and column as reference (it will not use the vals as input, it will use the 
// reference to return the results), takes the board array to allow it to make sure there's not
// already something in the requested spot
// Returns nothing directly
void playerInput(int &playerRow, int &playerColumn, int board[3][3])
{
	// Have to do some extra work because the columns are letters so we will need this var later
	char playerColumnChar;

	// Outer loop: repeats once and then until the player enters a valid row 
	// AND doesn't place over something else
    do {
    	// Ask user for their row selection
        cout << "What Row (1, 2, or 3) would you like to place your 'O' in?" << endl;
        // Take it 
        cin >> playerRow;
        cout << endl;
        // Subtract one because the user interface starts at 1 but our back-end stuff starts at 0
        playerRow--;

        // Inner loop: repeats once and then until the user enters a valid column
        do {
    		// Ask user for their column selection
            cout << "What Column (A, B, or C) would you like to place your 'O' in?" << endl;
            // Take it
            cin >> playerColumnChar;
            cout << endl;

            // Process to make it a valid number
            // If it's lowercase, subtract 32 to make it upper
            if (islower(playerColumnChar)) {
            	playerColumnChar -= 32;
            }
            // Transfer the value to the int var
            playerColumn = playerColumnChar;
            // Subtract 65 (A -> 0, B -> 1, C -> 2) 
            playerColumn -= 65;

            // Error message if the column is out of range
            if (playerColumn < 0 || playerColumn > 2) {
            	cout << "Slow down there buddy, that wasn't in the range we asked for. ";
            	cout << "Do better next time. \nOr else..." << endl;
            }

        // Repeat until the user enters valid stuff (in column range)
        } while(playerColumn < 0 || playerColumn > 2);

        // Error message if the row is out of range
        if (playerRow < 0 || playerRow > 2) {
        	cout << "Whoa there, that wasn't in the range we asked for. We'll give you another chance." << endl;
        }
        // Error message if theres already something at that spot on the board
        // Else if because we don't want to get here if we'll be reading random vals
        else if (board[playerRow][playerColumn]) {
        	cout << "HEY, watch it! There's already something there. Try again." << endl;
        }

    // Repeat until the user enters valid stuff (in row range and not on top of something else)
    } while(playerRow < 0 || playerRow > 2 || board[playerRow][playerColumn]);

    // Extra line for breathing room
    cout << endl;
}

// In our original approach, we used this function a lot
// The new method only uses it once, but we already had it so we kept it
// It takes a row and a column, both by reference
// It does not return anything directly 
void incCoords(int& row, int& col) 
{
	// If the column is 2, we're at the end of the row
	// so we increment the row and set the column to 0
	if (col == 2) {
		row++;
		col = 0;
	}
	// Otherwise, just increment the column
	else {
		col++;
	}
}

// This is the play that the computer does for the first round
// Takes the row and column (it won't use it for inputs, it will use them
// to return the choices) and the board array which it will use to make
// sure it's not placing over something else
void compDumbPlay(int &compRow, int &compCol, int board[3][3])
{

	// Outer loop: repeats once and then until the choice is in row range 
	// AND doesn't place over something else	
    do {
    	// Chooses a random number 0-2, inclusive
        compRow = rand() % 3; 

        // Inner loop: repeats once and then until the choice is in column range
        do {
    		// Chooses a random number 0-2, inclusive
        	compCol = rand() % 3; 

        // Repeat until the choice is in the column range
        } while(compCol < 0 || compCol > 2);

    // Repeat until the choice is in the row range
    } while(compRow < 0 || compRow > 2 || board[compRow][compCol]);
}

// This func checks to see if there is a win, tie, loss, or nothing on the board
// The func takes just the board array and returns and integer indicating the result
// 0 (normal operation-nothing happened), 1 (player win), 2 (computer win), 
// or 3 (tie)
int winTieLoss(int board[3][3]) 
{
	// check is the var we will return at the end
	int check = 0;
	// temp is used when we are reshuffling the array
	int temp = 0;
	// fullSpots is used when we are figuring out how many spots are filled
	int fullSpots = 0;
	// Make a copy of the board because we were having some strange behavior when we didn't
	// This just makes a new array of the same size
	int boardCopy[3][3];
	// These loops both copy our board to the new array and count how many spots are
	// filled so we can check for a tie later
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			boardCopy[i][j] = board[i][j];
			// If there's anything other than a 0 (which means the spot is empty),
			// add one to the count
			if (boardCopy[i][j]) {
				fullSpots++;
			}
		}
	}

	// This loop goes through each row and checks if, for that row, all of the spots are the same
	// If so, check is set to one of the spots (this works because the spot values are the same for 
	// each player as the value that indicates that they win)
	for (int i = 0; i < 3; i++) {
		if (boardCopy[i][0] == boardCopy[i][1] && boardCopy[i][1] == boardCopy[i][2] && boardCopy[i][0]) {
			check = boardCopy[i][0];
		}
	}

	// Same thing here, but it checks each column
	for (int i = 0; i < 3; i++) {
		if (boardCopy[0][i] == boardCopy[1][i] && boardCopy[1][i] == boardCopy[2][i] && boardCopy[0][i]) {
			check = boardCopy[0][i];
		}
	}

	// This loop reshuffles the board by moving each row by one more than the last
	// 123           312
	// 123  becomes  231
	// 123           123
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < i + 1; j++) {
			temp = boardCopy[i][2];
			boardCopy[i][2] = boardCopy[i][1];
			boardCopy[i][1] = boardCopy[i][0];
			boardCopy[i][0] = temp;
		}
	}

	// We can then check the first column to see if theres a bottom-left to top-right win
	// If there is, check is set to one of the values in the winning diagonal
	if (boardCopy[0][0] == boardCopy[1][0] && boardCopy[1][0] == boardCopy[2][0] && boardCopy[0][0]) {
		check = boardCopy[0][0];
	}

	// This loop does the same thing but twice and in the opposite direction
	// 312           123
	// 231  becomes  231
	// 123           312
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2 * i + 1; j++) {
			temp = boardCopy[i][0];
			boardCopy[i][0] = boardCopy[i][1];
			boardCopy[i][1] = boardCopy[i][2];
			boardCopy[i][2] = temp;
		}
	}

	// Then we check the first column for a match in the other diagonal
	// If there is, check is set to one of the values in the winning diagonal
	if (boardCopy[0][0] == boardCopy[1][0] && boardCopy[1][0] == boardCopy[2][0] && boardCopy[0][0]) {
		check = boardCopy[0][0];
	}

	// If we haven't found a win yet and all spots are full, it's a draw so check is 3
	if (check == 0 && fullSpots == 9) {
		check = 3;
	}

	// return check
	return check;
}

// This function is not assuming optimal play from the player; rather it is 
// just trying to make sure the inputs allow some path to win or draw
// compSim takes a potential row and column index and the board
// It will return a bool; true (it's a good play) or false (bad play)
bool compSim(int compRow, int compCol, int board[3][3])
{
	// New array with the same size because we will be altering it and sometimes need to revert
	int boardCopy[3][3];
	// We will use these vars later
	int playerRow, playerCol;

	// Make the copy
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			boardCopy[i][j] = board[i][j];
		}
	}

	// Make the play requested by the calling func for the computer
	// Don't print it and use our boardCopy
	updateBoard(compRow, compCol, 2, boardCopy, false);

	// We'll test the simulation 3 times
	for (int k = 0; k < 3; k++) {
		// The outer loop will repeat until there is a result
		do {
			// The middle loop will repeat until a valid row is chosen
		    do {
		    	// Choose a random row, 0-2
		        playerRow = rand() % 3;

		        // Inner loop repeats until a valid row is chosen
		        do {
		        	// Choose a random column, 0-2
		        	playerCol = rand() % 3; 

		       	// Keep going until the column is in range
		        } while (playerCol < 0 || playerCol > 2);

		    // Keep going until the row is in range and there's not already something there
		    } while (playerRow < 0 || playerRow > 2 || boardCopy[playerRow][playerCol]);
		    // Update the board with the player's choice, don't print
		    updateBoard(playerRow, playerCol, 1, boardCopy, false);

			if (winTieLoss(boardCopy)) {
				if (winTieLoss(boardCopy) != 1) {
					return true;
				}
				else {
					goto label;
				}
			}

			// The middle loop will repeat until a valid row is chosen
		    do {
		    	// Choose a random row, 0-2
		        compRow = rand() % 3;

		        // Inner loop repeats until a valid row is chosen
		        do {
		        	// Choose a random column, 0-2
		        	compCol = rand() % 3; 

		       	// Keep going until the column is in range
		        } while (compCol < 0 || compCol > 2);

		    // Keep going until the row is in range and there's not already something there
		    } while (compRow < 0 || compRow > 2 || boardCopy[compRow][compCol]);
		    // Update the board with the computer's choice, don't print
		    updateBoard(compRow, compCol, 2, boardCopy, false);

		    // If there is a result:
			if (winTieLoss(boardCopy)) {
				// If there it was a comp win or tie, return true
				if (winTieLoss(boardCopy) != 1) {
					return true;
				}
				// Otherwise, use goto to get out of the loops
				// We can't use break because we are in multiple loops
				else {
					goto label;
				}
			}

		// Repeat until there is a result
		} while (!winTieLoss(boardCopy));

		// This is where we go when we break the loop
		label:
		// Reset boardCopy to be the same as the original board
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				boardCopy[i][j] = board[i][j];
			}
		}

	} 

	// If we get here, return true
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
			updateBoard(compRow, compCol, 1, boardCopy, false);
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

bool compCheck(int &compRow, int &compCol, int board[3][3]) {
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
			updateBoard(compRow, compCol, 2, boardCopy, false);
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

	if (compCheck(compRow, compCol, boardCopy)) {
		return true;
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

	} while (!compSim(compRow, compCol, boardCopy));

	return true;
}

string banter() {
	int numPhrases = 10;
	string phrases[numPhrases] = {
		"Ooooo.... that'll hurt!",
		"Good luck coming back from that...",
		"oof",
		"Hmmm. Interesting move.",
		"Thinking you'll regret that...",
		"Not sure about that move, chief.",
		"Yikes!",
		"May as well restart...",
		"I'm at a loss for words...",
		"*Crowd gasps*"
	};

	string line = phrases[rand() % numPhrases];

	return line;
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
				cout << "Computer says:" << endl;
			}
			else {
				compDumbPlay(compRow, compCol, board);			
			}
			if (updateBoard(compRow, compCol, 2, board, true)) {
				break;
			}

			playerInput(playerRow, playerCol, board);
			cout << banter() << endl;
			if (updateBoard(playerRow, playerCol, 1, board, true)) {
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

		cout << "So...  Do you want to play again?" << endl;
		do {
			cout << "'Y' or 'N'" << endl;
			cin >> token;

		} while (token != 'Y' && token != 'N' && token != 'y' && token != 'n');
		cout << endl;

	} while(token == 'Y' || token == 'y');

    return 0;
}
