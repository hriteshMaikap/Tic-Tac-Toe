#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define SIZE 3
#define PLAYER_X 1
#define PLAYER_O 2

int board[SIZE][SIZE] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
int player, computer;
int maxDepth;

void printBoard() {
    printf("\nCurrent Board:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0)
                printf(" . ");
            else if (board[i][j] == PLAYER_X)
                printf(" X ");
            else
                printf(" O ");
        }
        printf("\n");
    }
}

bool checkWin(int player) {
    // Check rows and columns
    for (int i = 0; i < SIZE; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
            return true;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player)
            return true;
    }

    // Check diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
        return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
        return true;

    return false;
}

bool isMovesLeft() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                return true;
            }
        }
    }
    return false;
}

int evaluate() {
    if (checkWin(player))
        return -10;
    if (checkWin(computer))
        return 10;
    return 0;
}

int minimax(int depth, bool isMax) {
    int score = evaluate();

    if (score == 10 || score == -10)
        return score;
    if (!isMovesLeft())
        return 0;
    if (depth >= maxDepth)
        return 0;

    if (isMax) {
        int best = INT_MIN;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = computer;
                    best = (best > minimax(depth + 1, !isMax)) ? best : minimax(depth + 1, !isMax);
                    board[i][j] = 0;
                }
            }
        }
        return best;
    } else {
        int best = INT_MAX;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = player;
                    best = (best < minimax(depth + 1, !isMax)) ? best : minimax(depth + 1, !isMax);
                    board[i][j] = 0;
                }
            }
        }
        return best;
    }
}

void findBestMove(int *bestMoveRow, int *bestMoveCol) {
    int bestVal = INT_MIN;
    *bestMoveRow = -1;
    *bestMoveCol = -1;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                board[i][j] = computer;
                int moveVal = minimax(0, false);
                board[i][j] = 0;
                if (moveVal > bestVal) {
                    *bestMoveRow = i;
                    *bestMoveCol = j;
                    bestVal = moveVal;
                }
            }
        }
    }
}

void playerMove() {
    int move;
    printf("Enter your move (1-9): ");
    scanf("%d", &move);
    move--; // 0 based indexing
    int row = move / SIZE;
    int col = move % SIZE;
    if (board[row][col] == 0)
        board[row][col] = player;
    else {
        printf("Invalid move. Try again.\n");
        playerMove();
    }
}

void computerMove() {
    int bestMoveRow, bestMoveCol;
    findBestMove(&bestMoveRow, &bestMoveCol);
    if (bestMoveRow != -1 && bestMoveCol != -1) {
        board[bestMoveRow][bestMoveCol] = computer;
    }
}

int main() {
    char choice;
    printf("Do you want to be X or O? ");
    scanf(" %c", &choice);

    if (choice == 'X' || choice == 'x') {
        player = PLAYER_X;
        computer = PLAYER_O;
    } else {
        player = PLAYER_O;
        computer = PLAYER_X;
    }

    printf("Choose difficulty level (1 - Easy, 2 - Medium, 3 - Hard): ");
    int difficulty;
    scanf("%d", &difficulty);
    if (difficulty == 1)
        maxDepth = 1;
    else if (difficulty == 2)
        maxDepth = 3;
    else
        maxDepth = 9;

    int turn = 1; // 1 for player, 2 for computer
    int moves = 0;
    while (1) {
        printBoard();
        if ((turn == 1 && player == PLAYER_X) || (turn == 2 && player == PLAYER_O)) {
            playerMove();
            if (checkWin(player)) {
                printBoard();
                printf("Player wins!\n");
                break;
            }
            turn = (turn == 1) ? 2 : 1;
        } else {
            computerMove();
            if (checkWin(computer)) {
                printBoard();
                printf("Computer wins!\n");
                break;
            }
            turn = (turn == 1) ? 2 : 1;
        }
        moves++;
        if (moves == 9) {
            printBoard();
            printf("It's a draw!\n");
            break;
        }
    }
    return 0;
}
