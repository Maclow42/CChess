#include "gameboard.h"

#include <stdarg.h>
#include <string.h>


void strcatf(char *dest, const char *format, ...) {
    /*
        * This function is used to concatenate a formatted string to another string
        * args:
        *  - dest: the string to concatenate to
        * - format: the format of the string to concatenate
        * - ...: the arguments to the format string
    */
    va_list args;
    va_start(args, format);

    // Use vsnprintf to get the size of the formatted string
    int size = vsnprintf(NULL, 0, format, args);
    va_end(args);

    // Allocate enough memory for the formatted string
    char *formatted_str = (char *)malloc(size + 1);

    va_start(args, format);
    vsnprintf(formatted_str, size + 1, format, args);
    va_end(args);

    // Concatenate the formatted string to dest
    strcat(dest, formatted_str);

    // free the memory allocated by vsnprintf
    free(formatted_str);
}


void print_piece(char* square, piece* piece){
    if(piece == NULL)
        sprintf(square, "%s", "   ");
    else
        switch (piece->type){
            case PAWN:
                sprintf(square, piece->color == BLACK ? " ♙ " : " ♟ ");
                break;
            case ROCK:
                sprintf(square, piece->color == BLACK ? " ♖ " : " ♜ ");
                break;
            case KNIGHT:
                sprintf(square, piece->color == BLACK ? " ♘ " : " ♞ ");
                break;
            case BISHOP:
                sprintf(square, piece->color == BLACK ? " ♗ " : " ♝ ");
                break;
            case QUEEN:
                sprintf(square, piece->color == BLACK ? " ♕ " : " ♛ ");
                break;
            case KING:
                sprintf(square, piece->color == BLACK ? " ♔ " : " ♚ ");
                break;
            default:
                sprintf(square, "   ");
                break;
        }
}

void printBoard_as_WHITE(char** result, game_board* board){
    sprintf(result[0], "    A   B   C   D   E   F   G   H");
    sprintf(result[1], "  ┌───┬───┬───┬───┬───┬───┬───┬───┐");
    for(int j = 7; j >= 0; j--){
        if(j != 7)
            sprintf(result[2*(7-j)+1], "  ├───┼───┼───┼───┼───┼───┼───┼───┤");

        char line[102] = "";
        strcatf(line, "%i │", j+1);
        for(int i = 0; i < 8; i++){
            char tmp[6] = "";
            print_piece(tmp, board->board[i][j]);
            strcatf(line, tmp);
            if(i == 7){
                strcatf(line, "│ %i", j+1);
            }else{
                strcatf(line, "│");
            }
        }
        sprintf(result[2*(7-j)+2], "%s", line);
    }
    sprintf(result[17], "  └───┴───┴───┴───┴───┴───┴───┴───┘");
    sprintf(result[18], "    A   B   C   D   E   F   G   H");
}

void printBoard_as_BLACK(char** result, game_board* board){
    sprintf(result[0], "    A   B   C   D   E   F   G   H  ");
    sprintf(result[1], "  ┌───┬───┬───┬───┬───┬───┬───┬───┐");
    for(int j = 0; j < 8; j++){
        if(j != 0)
            sprintf(result[2*j+1], "  ├───┼───┼───┼───┼───┼───┼───┼───┤");

        char line[102] = "";
        strcatf(line, "%i │", j+1);
        for(int i = 0; i < 8; i++){
            char tmp[6] = "";
            print_piece(tmp, board->board[i][j]);
            strcatf(line, tmp);
            if(i == 7){
                strcatf(line, "│ %i", j+1);
            }else{
                strcatf(line, "│");
            }
        }
        sprintf(result[2*j+2], "%s", line);
    }
    sprintf(result[17], "  └───┴───┴───┴───┴───┴───┴───┴───┘");
    sprintf(result[18], "    A   B   C   D   E   F   G   H");
}

void printBoard(game_board* board, enum color pov, char** result){
    if(pov == WHITE)
        printBoard_as_WHITE(result, board);
    else
        printBoard_as_BLACK(result, board);
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