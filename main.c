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
        
        movePiece(board, currentx, currenty, tox, toy);
    }
}

int main(int argc, char **argv) {
    piece board[8][8] = {NONE_PIECE};
        board[0][0] = newPiece(ROCK, WHITE);
        board[0][2] = newPiece(ROCK, WHITE);
        board[1][5] = newPiece(ROCK, WHITE);
        board[3][5] = newPiece(ROCK, WHITE);
        board[2][1] = newPiece(KING, BLACK);

    char current_pos[2], to_pos[2];
    int currentx, currenty, tox, toy;
    while(1){
        printf("\e[1;1H\e[2J");
        printBoard(board);

        int kingx = 0, kingy = 0;
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                if(board[i][j].type == KING){
                    kingx = i;
                    kingy = j;
                }
            }
        }
        printf("isChess : %i\n", isInChess(board, kingx, kingy));
        printf("isMate : %i\n", isMate(board, kingx, kingy));

        printf("Enter a move : ");
        scanf("%s %s", &current_pos, &to_pos);

        currentx = current_pos[0] - 'A';
        currenty = current_pos[1] -'1';
        tox = to_pos[0] - 'A';
        toy = to_pos[1] - '1';
        
        movePiece(board, currentx, currenty, tox, toy);
    }
}