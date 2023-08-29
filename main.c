#include <stdio.h>
#include "board.h"
#include "alpha_beta.h"

int main() {
    game_board *board = newBoard();
    initGameBoard(board);


    char* getted_current_pos = malloc(sizeof(char)*2);
    char* getted_to_pos = malloc(sizeof(char)*2);

    while(1){
        printf("\e[1;1H\e[2J");
        printBoard(board);

        printf("to play : %s\n", board->to_play==WHITE?"white":"black");

        if(board->to_play == WHITE){
            printf("Enter a move : ");
            int scan_result = scanf("%s %s", getted_current_pos, getted_to_pos);
            if(scan_result == 1)
                return false;

            coords current_pos = {getted_current_pos[0] - 'A', getted_current_pos[1] -'1'};
            coords to_pos = {getted_to_pos[0] - 'A', getted_to_pos[1] - '1'};
            
            if(playerMovePiece(board, current_pos, to_pos)){
                game_status status = getGameStatus(board);
                if(status == WHITE_MATE || status == BLACK_MATE){
                    printf("\e[1;1H\e[2J");
                    printBoard(board);
                    printf("Checkmate !\n");
                    break;
                }
                else if(status == PAT){
                    printf("\e[1;1H\e[2J");
                    printBoard(board);
                    printf("Pat !\n");
                    break;
                }
            }
        }
        else{
            movement_coords* best_move = getBestMove(board, 4);
            coords start_pos = best_move->start_pos;
            coords end_pos = best_move->end_pos;
            playerMovePiece(board, start_pos, end_pos);
            
            game_status status = getGameStatus(board);
                if(status == WHITE_MATE || status == BLACK_MATE){
                    printf("\e[1;1H\e[2J");
                    printBoard(board);
                    printf("Checkmate !\n");
                    break;
                }
                else if(status == PAT){
                    printf("\e[1;1H\e[2J");
                    printBoard(board);
                    printf("Pat !\n");
                    break;
                }
        }
    }
    freeBoard(board);
}