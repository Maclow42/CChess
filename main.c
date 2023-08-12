#include <stdio.h>
#include "board.h"

void play(){
    piece board[8][8];
    newBoard(board);

    char current_pos[2], to_pos[2];
    int currentx, currenty, tox, toy;

    while(1){
        printf("\e[1;1H\e[2J");
        printBoard(board);

        printf("Enter a move : ");
        scanf("%s %s", &current_pos, &to_pos);

        currentx = current_pos[0] - 'A';
        currenty = current_pos[1] -'1';
        tox = to_pos[0] - 'A';
        toy = to_pos[1] - '1';
        
        movePiece(board, Coords(currentx, currenty), Coords(tox, toy));
    }
}

int main(int argc, char **argv) {
    game_board *board = malloc(sizeof(game_board));

    board->board = (piece **)malloc(sizeof(piece *) * 8);
    for(int i = 0; i < 8; i++)
        board->board[i] = (piece *)malloc(sizeof(piece) * 8);

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++)
            board->board[i][j] = newPiece(NONE_PIECE, NONE_COLOR);
    }
        
    board->board[0][0] = newPiece(ROCK, WHITE);
    board->board[0][2] = newPiece(ROCK, WHITE);
    board->board[1][5] = newPiece(ROCK, WHITE);
    board->board[4][5] = newPiece(ROCK, WHITE);
    board->board[2][1] = newPiece(KING, BLACK);
    board->board[7][7] = newPiece(KING, WHITE);

    board->black_king_pos = Coords(2, 1);
    board->white_king_pos = Coords(7, 7);

    char current_pos[2], to_pos[2];
    int currentx, currenty, tox, toy;
    while(1){
        printf("\e[1;1H\e[2J");
        printBoard(board);

        printf("Game status : %i\n", (int) getGameStatus(board));

        printf("Enter a move : ");
        scanf("%s %s", &current_pos, &to_pos);

        currentx = current_pos[0] - 'A';
        currenty = current_pos[1] -'1';
        tox = to_pos[0] - 'A';
        toy = to_pos[1] - '1';
        
        movePiece(board, Coords(currentx, currenty), Coords(tox, toy));
    }
}