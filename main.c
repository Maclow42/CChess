#include <stdio.h>
#include "board.h"
#include "alpha_beta.h"

int main() {
    game_board *board = newBoard();
    board->board[5][7] = newBishop(BLACK);
    board->board[7][8] = newRock(BLACK);
    board->board[0][6] = newPawn(BLACK);
    board->board[3][5] = newPawn(BLACK);
    board->board[6][5] = newPawn(BLACK);
    board->board[7][5] = newPawn(BLACK);
    board->board[1][3] = newPawn(BLACK);
    board->board[2][3] = newKing(BLACK);

    board->board[0][0] = newRock(WHITE);
    board->board[7][0] = newRock(WHITE);
    board->board[0][1] = newPawn(WHITE);
    board->board[3][1] = newKing(WHITE);
    board->board[7][1] = newPawn(WHITE);
    board->board[1][2] = newPawn(WHITE);
    board->board[2][2] = newPawn(WHITE);
    board->board[5][3] = newPawn(WHITE);
    board->board[3][4] = newKnight(WHITE);
    board->board[5][4] = newPawn(WHITE);
    board->board[0][5] = newKnight(WHITE);
    board->board[4][5] = newBishop(WHITE);

    board->black_king_pos = Coords(2, 3);
    board->white_king_pos = Coords(3, 1);

    board->to_play = BLACK;

    char* getted_current_pos = malloc(sizeof(char)*2);
    char* getted_to_pos = malloc(sizeof(char)*2);

    while(1){
        printf("\e[1;1H\e[2J");
        printBoard(board);

        printf("score : %d\n", evaluateBoard(board));
        printf("status : %i\n", getGameStatus(board));
        printf("to play : %s\n", board->to_play==WHITE?"white":"black");

        list_t* accessible = getAllPossibleMove(board, BLACK);
        list_node_t *node;
        list_iterator_t *it = list_iterator_new(accessible, LIST_HEAD);
        while((node = list_iterator_next(it))){
            print_MoveCoords(node->val);
        }
        printf("accessible : %d\n", accessible->len);
        list_iterator_destroy(it);

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
            movement_coords* best_move = getBestMove(board, 1);
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