//Including necessary libraries
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

// Need prototypes for these funcs because of the way the fucncs call each other,
// it wouldn't be possible to not
void compSmartPlay(int &compRow, int &compCol, int board[3][3]);
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
        	cout << "Whoa there, that wasn't in the range we asked for. ";
        	cout << "We'll give you another chance." << endl;
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
	if (col >= 2) {
		row++;
		col = 0;
	}
	// Otherwise, just increment the column
	else {
		col++;
	}

	// Just as a failsafe, if the row is out of range, reset to 0,0
	if (row >= 3) {
		row = 0;
		col = 0;
	}
}

// This is the play that the computer does for the first round
// Takes the row and column (it won't use it for inputs, it will use them
// to return the choices) and the board array which it will use to make
// sure it's not placing over something else
void compDumbPlay(int &compRow, int &compCol, int board[3][3])
{
	// These parallel arrays prioritize the most preferable spots on the board
	// This function was originally used more than just the first play, that's why
	// it includes all 9 spots
	int rowChoices[9] = {1, 0, 0, 2, 2, 0, 1, 1, 2};
	int colChoices[9] = {1, 0, 2, 0, 2, 1, 0, 2, 1};
	// Set our index to 0
	int j = 0;

	// Outer loop: repeats once and then until the choice is in row range 
	// AND doesn't place over something else	
    do {
    	// Chooses the next number
        compRow = rowChoices[j];

        // Inner loop: repeats once and then until the choice is in column range
        do {
    		// Chooses the next number
        	compCol = colChoices[j];
        	// Increment j for next time
        	j++;

        // Just as a failsafe, make sure the column is in range
        } while(compCol < 0 || compCol > 2);

    // Just as a failsafe, make sure the column is in range and make 
    // sure there's not something there yet
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

// This func takes the vars to store the row and column, as well as the board
// It will return true (use the returned spot or the player will win) or 
// false (no immediate path to victory for player)
bool playerCheck(int &compRow, int &compCol, int board[3][3]) 
{
	// Like previous functions, we will make a copy because we will be altering it
	int boardCopy[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			boardCopy[i][j] = board[i][j];
		}
	}

	// We'll be checking each spot, so we start at 0,0
	compRow = 0;
	compCol = 0;

	// We will repeat 9 times, once for each spot
	for (int i = 0; i < 9; i++) {
		//If there's not already something in our spot, update it with the player's value
		if (!boardCopy[compRow][compCol]) {
			updateBoard(compRow, compCol, 1, boardCopy, false);
		}

		// If there's a result, it means the player wins, we don't want that so we return true 
		// which tells the calling function to use the values it returned for row and col
		if (winTieLoss(boardCopy)) {
			return true;
		}

		// Increment the spot
		incCoords(compRow, compCol);

		// Reset the board for next time
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				boardCopy[i][j] = board[i][j];
			}
		}		
	}

	// If we got here, there's no immediate path for the player, so we'll 
	// return false and use a different method
	return false;
}

// This func will check to see if there's an immediate path for the computer to win
// It takes the vars to store the row and column, as well as the board
// It will return true (use the returned spot to win) or 
// false (no immediate path to victory)
bool compCheck(int &compRow, int &compCol, int board[3][3]) 
{
	// Like previous functions, we will make a copy because we will be altering it
	int boardCopy[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			boardCopy[i][j] = board[i][j];
		}
	}

	// We'll be checking each spot, so we start at 0,0
	compRow = 0;
	compCol = 0;

	// We will repeat 9 times, once for each spot
	for (int i = 0; i < 9; i++) {
		//If there's not already something in our spot, update it with the comp's value
		if (!boardCopy[compRow][compCol]) {
			updateBoard(compRow, compCol, 2, boardCopy, false);
		}

		// If there's a result, it means the computer wins, so we return true
		// which tells the calling function to use the values it returned for row and col
		if (winTieLoss(boardCopy)) {
			return true;
		}

		// Increment the spot
		incCoords(compRow, compCol);

		// Reset the board for next time
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				boardCopy[i][j] = board[i][j];
			}
		}		
	}

	// If we got here, there's no immediate, so we'll 
	// return false and use a different method
	return false;
}

// This func selects our approach for the play
// Like usual it takes the vars for the row and column, ans well as the board
// Returns nothing
void compSmartPlay(int &compRow, int &compCol, int board[3][3])
{
	// Like previous functions, we will make a copy because we will be altering it
	int boardCopy[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			boardCopy[i][j] = board[i][j];
		}
	}

	// First see if we can quickly win
	// If so, return with the column and row changed
	if (compCheck(compRow, compCol, boardCopy)) {
		return;
	}

	// Next, we'll see if there's any immediate threat from the player
	// If so, return with the row and column set
	if (playerCheck(compRow, compCol, boardCopy)) {
		return;
	}

	// Next, we'll try to make a play that puts us on the path to victory
	do {
	    do {
	    	// Pick a random number 0, 1, or 2 for the row
	        compRow = rand() % 3;

	        do {
	        	// Pick a random number 0, 1, or 2 for the column
	        	compCol = rand() % 3; 

	        // Repeat the column picking until it's in range (just a backup to check range)
	        } while (compCol < 0 || compCol > 2);

	    // Check to make sure that the row choice in range and repeat until it picks 
	    // a spot that's not already occupied
	    } while (compRow < 0 || compRow > 2 || boardCopy[compRow][compCol]);

	// Run the compSim function and repeat the above process until compSim says it's a good choice
	} while (!compSim(compRow, compCol, boardCopy));
}

// This func is just for fun
// It has no inputs, but when called it returns a string to taunt the 
// player when they make their play
string banter() {
	// Size of the phrase string (number of strings)
	int numPhrases = 10;
	// These are all of the possible choices in an array
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

	// Set the variable string equal to a random index of the phrases array
	string line = phrases[rand() % numPhrases];

	// Return the phrase
	return line;
}

int main()
{	
	// This is the variable that the user uses to say if they want to play again
	char token;

	// This loop repeats once and then until the user says they don't want to play anymore
	do {
		// Set the seed to an arbitrary value to make the random generation more random
		srand(time(NULL));

		// Declare the board as a 3x3 array with all zeros
		int board[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
		// These are the variables we will be using
		int playerRow, playerCol, compRow, compCol, round = 0;

		// Tell the player that the computer will be going first
		cout << "Computer goes first:" << endl;

		// We will always repeat until we hit a break statement 
		while (true) {
			// Round starts at 0, so if it's after the first round, use smartPlay
			if (round) {
				compSmartPlay(compRow, compCol, board);
				// Let the user know that the following board is after the computer's choice
				cout << "Computer says:" << endl;
			}
			else {
				// Otherwise (the first round), use dumbPlay
				compDumbPlay(compRow, compCol, board);			
			}
			// Update the board using the row and col selected above 
			// updateBoard updates the variable, prints the board, 
			// and returns a non-zero if there is a result
			// If there's a result, we break the loop
			if (updateBoard(compRow, compCol, 2, board, true)) {
				break;
			}

			// Now it's the player's turn
			// call playerInput which will request and take the user's choice
			playerInput(playerRow, playerCol, board);
			// Output a phrase 
			cout << banter() << endl;
			// Update the board using the row and col selected above 
			// updateBoard updates the variable, prints the board, 
			// and returns a non-zero if there is a result
			// If there's a result, we break the loop
			if (updateBoard(playerRow, playerCol, 1, board, true)) {
				break;
			}

			// Add one to the round counter
			round++;
		}

		// We got here, so there's a result
		// Select a message based on the result
		switch (winTieLoss(board)) {
			// 1 means the player won
			case 1: 
				cout << "Congrats, you won!" << endl;
			break;
			// 2 means the computer won
			case 2:
				cout << "COMPUTERS ARE SUPERIOR" << endl;
			break;
			// 3 means it was a tie
			case 3:
				cout << "Dang! It was a tie!" << endl;
			break;
			// It should never be anything else
			default:
				cout << "FATAL ERROR" << endl;
		}

		// Ask if the user wants to play again
		cout << "So...  Do you want to play again?" << endl;
		// Repeat the following once and again until they enter a valid response
		do {
			// Give them the choices and take their input
			cout << "'Y' or 'N'" << endl;
			cin >> token;

		// Keep asking until they give a valid input
		} while (token != 'Y' && token != 'N' && token != 'y' && token != 'n');
		cout << endl;

	// Repeat the game as long as the user enters Y or y
	} while(token == 'Y' || token == 'y');

	// Done
    return 0;
}
