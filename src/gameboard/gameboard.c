#include "gameboard.h"

#include "accessible_pos.h"

void print_piece(piece* piece){
    if(piece == NULL)
        printf("%s", "   ");
    else
        switch (piece->type){
            case PAWN:
                printf("%s", piece->color == BLACK ? " ♙ " : " ♟ ");
                break;
            case ROCK:
                printf("%s", piece->color == BLACK ? " ♖ " : " ♜ ");
                break;
            case KNIGHT:
                printf("%s", piece->color == BLACK ? " ♘ " : " ♞ ");
                break;
            case BISHOP:
                printf("%s", piece->color == BLACK ? " ♗ " : " ♝ ");
                break;
            case QUEEN:
                printf("%s", piece->color == BLACK ? " ♕ " : " ♛ ");
                break;
            case KING:
                printf("%s", piece->color == BLACK ? " ♔ " : " ♚ ");
                break;
            default:
                printf("%s", "   ");
                break;
        }
}

void printBoard_as_WHITE(game_board* board){

    printf("     A   B   C   D   E   F   G   H\n");
    printf("   ┌───┬───┬───┬───┬───┬───┬───┬───┐\n");
    for(int j = 7; j >= 0; j--){
        if(j != 7)
            printf("   ├───┼───┼───┼───┼───┼───┼───┼───┤\n");
        printf(" %i %s", j+1, "│");
        for(int i = 0; i < 8; i++){
            print_piece(board->board[i][j]);
            if(i == 7)
                printf("│ %i\n", j+1);
            else
                printf("│");
        }
    }
    printf("   └───┴───┴───┴───┴───┴───┴───┴───┘\n");
    printf("     A   B   C   D   E   F   G   H\n");
}

void printBoard_as_BLACK(game_board* board){

    printf("     A   B   C   D   E   F   G   H\n");
    printf("   ┌───┬───┬───┬───┬───┬───┬───┬───┐\n");
    for(int j = 0; j < 8; j++){
        if(j != 0)
            printf("   ├───┼───┼───┼───┼───┼───┼───┼───┤\n");
        printf(" %i %s", j+1, "│");
        for(int i = 0; i < 8; i++){
            print_piece(board->board[i][j]);
            if(i == 7)
                printf("│ %i\n", j+1);
            else
                printf("│");
        }
    }
    printf("   └───┴───┴───┴───┴───┴───┴───┴───┘\n");
    printf("     A   B   C   D   E   F   G   H\n");
}

void printBoard(game_board* board, enum color pov){
    if(pov == WHITE)
        printBoard_as_WHITE(board);
    else
        printBoard_as_BLACK(board);
}

game_board* newBoard(){
    game_board *board = malloc(sizeof(game_board));

    board->board = malloc(sizeof(piece **) * 8);
    for(int i = 0; i < 8; i++)
        board->board[i] = malloc(sizeof(piece*) * 8);

    return board;
}

void freeBoard(game_board *board) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if(board->board[i][j] != NULL)
                free(board->board[i][j]);
        }
        free(board->board[i]); // Free row of the board
    }
    free(board->board); // Free the entire board

    free(board); // Free the game_board structure itself
}


void initGameBoard(game_board *board){
    board->moves_stack = stack_new();

    board->white_king_pos = (coords) {4, 0};
    board->black_king_pos = (coords) {4, 7};

    board->nb_piece = 32;

    board->white_castled = true;
    board->black_castled = true;

    board->to_play = WHITE;

    board->last_move_type = NORMAL_MOVE;

    board->board[0][0] = newRock(WHITE);
    board->board[1][0] = newKnight(WHITE);
    board->board[2][0] = newBishop(WHITE);
    board->board[3][0] = newQueen(WHITE);
    board->board[4][0] = newKing(WHITE);
    board->board[5][0] = newBishop(WHITE);
    board->board[6][0] = newKnight(WHITE);
    board->board[7][0] = newRock(WHITE);
    for(int i = 0; i < 8; i++)
        board->board[i][1] = newPawn(WHITE);

    for(int j = 2; j < 6; j++){
        for(int i = 0; i < 8; i++)
            board->board[i][j] = NULL;
    }

    for(int i = 0; i < 8; i++)
        board->board[i][6] = newPawn(BLACK);
    board->board[0][7] = newRock(BLACK);
    board->board[1][7] = newKnight(BLACK);
    board->board[2][7] = newBishop(BLACK);
    board->board[3][7] = newQueen(BLACK);
    board->board[4][7] = newKing(BLACK);
    board->board[5][7] = newBishop(BLACK);
    board->board[6][7] = newKnight(BLACK);
    board->board[7][7] = newRock(BLACK);
}