#include "board.h"
#include "src/list/list.h"

bool isPosAccessible_Pawn(piece** board, int currentx, int currenty, int tox, int toy){
    // forward move of one case
    if(toy == currenty + (board[currentx][currenty].color == WHITE?1:-1)){
        if(tox == currentx)
            return board[tox][toy].type == NONE_PIECE;
        // move diagonaly to catch a piece
        else if(abs(tox - currentx) == 1)
            return board[tox][toy].type != NONE_PIECE;
        
        return false;
    }

    // if it's the first move of the pawn, it can move of 2 cases
    if(board[currentx][currenty].first_move){
        if(toy == currenty + 2 && tox == currentx){
                board[currentx][currenty].first_move = false;
                return board[tox][toy].type == NONE_PIECE;
        }
    }

    return false;
}

bool isPosAccessible_Rock(piece** board, int currentx, int currenty, int tox, int toy){
    if(toy != currenty && tox != currentx)
        return false;
    
    // if the asked deplacement is vertical
    if(tox == currentx){
        for(int i = currenty+(toy>currenty?1:-1); i != toy; toy>currenty?i++:i--){
            if(board[currentx][i].type != NONE_PIECE)
                return false;
        }
    }

    // else the deplacement is horizontal
    else{
        for(int i = currentx+(tox>currentx?1:-1); i != tox; tox>currentx?i++:i--){
            if(board[i][currenty].type != NONE_PIECE)
                return false;
        }
    }

    return true;
}

bool isAccessible_Knight(piece** board, int currentx, int currenty, int tox, int toy){
    // Array listing all relative positions accessible by a knight
    int relative_accessible[8][2] = {  {-2, 1},
                                        {-1, 2},
                                        {1, 2},
                                        {2, 1},
                                        {2, -1},
                                        {1, -2},
                                        {-1, -2},
                                        {-2, -2}
                                     };
    // check if one possibility corresponds to the asked deplacement
    for(int i = 0; i < 8; i++){
        if(currentx + relative_accessible[i][0] == tox && currenty + relative_accessible[i][1] == toy)
            return true;
    }

    return false;
}

bool isAccessible_Bishop(piece** board, int currentx, int currenty, int tox, int toy){
    if(abs(currentx - tox) != abs(currenty - toy))
        return false;

    int dist = abs(currentx - tox);

    int vect_x = abs(tox - currentx)/(tox - currentx);
    int vect_y = abs(toy - currenty)/(toy - currenty);

    for(int i = 1; i < dist; i++){
        if(board[currentx + vect_x*i][currenty + vect_y*i].type != NONE_PIECE)
            return false;
    }

    return true;
}

bool isAccessible_King(piece** board, int currentx, int currenty, int tox, int toy){
    if(abs(currentx - tox) > 1 || abs(currenty - toy) > 1)
        return false;

    return true;    
}

bool isAccessible_Queen(piece** board, int currentx, int currenty, int tox, int toy){
    return  isAccessible_Bishop(board, currentx, currenty, tox, toy) || \
            isPosAccessible_Rock(board, currentx, currenty, tox, toy);
}

bool isPosAccessible(game_board* board, coords* current_pos, coords* dest_pos){
    /*
    Return true if piece at (currentx,currenty) in board can be moved to (tox, toy), otherwise return false
    Arguments:
    */

   unsigned int currentx = current_pos->posx;
   unsigned int currenty = current_pos->posy;

   unsigned int tox = dest_pos->posx;
   unsigned int toy = dest_pos->posy;

    // if coords are not valid then return false
    if(!(areCoordsValid(current_pos) && areCoordsValid(dest_pos)))
        return false;

    // check if there actually is a piece at the indicated current position (if not return false)
    if(board->board[currentx][currenty].type == NONE_PIECE)
        return false;
    
    // check if the arrival position is not occupied by a piece of the same color
    if(board->board[tox][toy].color == board->board[currentx][currenty].color)
        return false;

    int (*functionTable[6])(piece** board, int currentx, int currenty, int tox, int toy) = { isPosAccessible_Pawn,
                                                                                                isPosAccessible_Rock,
                                                                                                isAccessible_Knight,
                                                                                                isAccessible_Bishop,
                                                                                                isAccessible_Queen,
                                                                                                isAccessible_King};
    return functionTable[board->board[currentx][currenty].type-1](board->board, currentx, currenty, tox, toy);
}

bool isPosAccessible_noPositionCheck(game_board* board, coords* current_pos, coords* dest_pos){
    /*
        Same as isPosAccessible but do not check if indicated positions are valids
        (private function, used chen position already checked)
    */

   int currentx = current_pos->posx, currenty = current_pos->posy;
   int tox = dest_pos->posx, toy = dest_pos->posy;

    // check if there actually is a piece at the indicated current position (if not return false)
    if(board->board[currentx][currenty].type == NONE_PIECE)
        return false;
    
    // check if the arrival position is not occupied by a piece of the same color
    if(board->board[tox][toy].color == board->board[currentx][currenty].color)
        return false;

    int (*functionTable[6])(piece** board, int currentx, int currenty, int tox, int toy) = { isPosAccessible_Pawn,
                                                                                                    isPosAccessible_Rock,
                                                                                                    isAccessible_Knight,
                                                                                                    isAccessible_Bishop,
                                                                                                    isAccessible_Queen,
                                                                                                    isAccessible_King};
    return functionTable[board->board[currentx][currenty].type-1](board->board, currentx, currenty, tox, toy);
}

bool movePiece(game_board* board, coords* current_pos, coords* dest_pos){
    // if the piece do not belong to the good player return false
    if(isPosAccessible(board, current_pos, dest_pos)){
        int currentx = current_pos->posx, currenty = current_pos->posy;
        int tox = dest_pos->posx, toy = dest_pos->posy;

        board->board[tox][toy] = board->board[currentx][currenty];
        board->board[currentx][currenty] = newPiece(NONE_PIECE, NONE_COLOR);

        if(board->board[tox][toy].type == KING){
            if(board->board[tox][toy].color == WHITE)
                board->white_king_pos = dest_pos;
            else
                board->black_king_pos = dest_pos;
        }

        return true;
    }

    return false;
}

bool playerMovePiece(game_board* board, coords* current_pos, coords* dest_pos){
    if(board->board[current_pos->posx][current_pos->posy].color != board->to_play)
        return false;
    
    piece tmp = board->board[dest_pos->posx][dest_pos->posy];

    if(!movePiece(board, current_pos, dest_pos))
        return false;

    game_status status = getGameStatus(board);
    if(board->to_play == WHITE){
        if(status == WHITE_CHESS || status == WHITE_MATE){
            movePiece(board, dest_pos, current_pos);
            board->board[dest_pos->posx][dest_pos->posy] = tmp;
            return false;
        }
    }

    else{
        if(status == BLACK_CHESS || status == BLACK_MATE){
            movePiece(board, dest_pos, current_pos);
            board->board[dest_pos->posx][dest_pos->posy] = tmp;
            return false;
        }
    }

    board->to_play = board->to_play == WHITE ? BLACK : WHITE;

    return true;
}


list_t* getPossiblePos(game_board* board, coords* current_pos){
    list_t *result = newList();

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++){
            coords *current_ij = Coords(i, j);
            if(isPosAccessible(board, current_pos, current_ij)){
                pushList(result, current_ij);
            }
        }
    
    return result;
}

int isInChess(game_board* board, coords* kingpos){
    int kingposx = kingpos->posx, kingposy = kingpos->posy;

    if(board->board[kingposx][kingposy].type != KING){
        fprintf(stderr, "No king found at position (%hi, %hi).\n", kingposx, kingposy);
        return false;
    }

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(isPosAccessible_noPositionCheck(board, Coords(i, j), kingpos))
                return true;
        }
    }

    return false;
}

bool isMate(game_board* board, coords* kingpos){
    // check if the king can move out of chess
    // Array listing all relative positions accessible by the king

    int kingposx = kingpos->posx, kingposy = kingpos->posy;

    int relative_accessible[8][2] = {   
                                        {0, 1},
                                        {1, 1},

                                        {1, 0},
                                        {1, -1},

                                        {0, -1},
                                        {-1, -1},

                                        {-1, 0},
                                        {-1, 1},
                                    };

    for(int i = 0; i < 8; i++){
        coords* dest_pos = Coords(kingposx+relative_accessible[i][0], kingposy+relative_accessible[i][1]);
        // if the king can move at the position
        piece tmp = board->board[dest_pos->posx][dest_pos->posy];
        if(movePiece(board, kingpos, dest_pos)){
            bool isChess = isInChess(board, dest_pos);
            movePiece(board, dest_pos, kingpos);
            board->board[dest_pos->posx][dest_pos->posy] = tmp;
            if(!isChess)
                return false;
        }
    }

    // check if a piece can eat or cover the ennemy
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(board->board[i][j].color == board->board[kingposx][kingposy].color && 10*i+j!= 10*kingposx+kingposy){
                // get all accessible positions
                list_t *access = getPossiblePos(board, Coords(i, j));
                // move the current piece to all accessible positions
                while(access->size!=0){
                    coords* dest_pos = (coords *)getIndexList(access, 0);
                    popList(access, 0);
                    piece tmp = board->board[dest_pos->posx][dest_pos->posy];
                    movePiece(board, Coords(i, j), dest_pos);
                    bool is_chess = isInChess(board, kingpos);
                    movePiece(board, dest_pos, Coords(i, j));
                    board->board[dest_pos->posx][dest_pos->posy] = tmp;
                    if(!is_chess)
                        return false;
                }
            }
        }
    }

    return true;

}

game_status getGameStatus(game_board *board)
{
    #if 0
    bool white_chess = isInChess(board, board->white_king_pos);
    bool white_mate = isMate(board, board->white_king_pos);

    if(white_chess){
        if(white_mate)
            return WHITE_MATE;
        return WHITE_CHESS;
    }
    if(white_mate)
        return WHITE_PAT;
    #endif

    bool black_chess = isInChess(board, board->black_king_pos);
    bool black_mate = isMate(board, board->black_king_pos);

    if(black_chess){
        if(black_mate)
            return BLACK_MATE;
        return BLACK_CHESS;
    }
    if(black_mate)
        return BLACK_PAT;

    return RAS;
}

void printBoard(game_board* board){

    printf("     A   B   C   D   E   F   G   H\n");
    printf("   ┌───┬───┬───┬───┬───┬───┬───┬───┐\n");
    for(int j = 7; j >= 0; j--){
        if(j != 7)
            printf("   ├───┼───┼───┼───┼───┼───┼───┼───┤\n");
        printf(" %i %s", j+1, "│");
        for(int i = 0; i < 8; i++){
            switch (board->board[i][j].type){
                case PAWN:
                    printf("%s", board->board[i][j].color == BLACK ? " ♙ " : " ♟ ");
                    break;
                case ROCK:
                    printf("%s", board->board[i][j].color == BLACK ? " ♖ " : " ♜ ");
                    break;
                case KNIGHT:
                    printf("%s", board->board[i][j].color == BLACK ? " ♘ " : " ♞ ");
                    break;
                case BISHOP:
                    printf("%s", board->board[i][j].color == BLACK ? " ♗ " : " ♝ ");
                    break;
                case QUEEN:
                    printf("%s", board->board[i][j].color == BLACK ? " ♕ " : " ♛ ");
                    break;
                case KING:
                    printf("%s", board->board[i][j].color == BLACK ? " ♔ " : " ♚ ");
                    break;
                default:
                    printf("%s", "   ");
                    break;
            }
            if(i == 7)
                printf("│ %i\n", j+1);
            else
                printf("│");
        }
    }
    printf("   └───┴───┴───┴───┴───┴───┴───┴───┘\n");
    printf("     A   B   C   D   E   F   G   H\n");
}

game_board* newBoard(){
    game_board *board = malloc(sizeof(game_board));

    board->board = (piece **)malloc(sizeof(piece *) * 8);
    for(int i = 0; i < 8; i++)
        board->board[i] = (piece *)malloc(sizeof(piece) * 8);

    return board;
}

void initGameBoard(game_board *board){
    board->white_king_pos = Coords(4, 0);
    board->black_king_pos = Coords(4, 7);

    board->to_play = WHITE;

    board->board[0][0] = newPiece(ROCK, WHITE);
    board->board[1][0] = newPiece(KNIGHT, WHITE);
    board->board[2][0] = newPiece(BISHOP, WHITE);
    board->board[3][0] = newPiece(QUEEN, WHITE);
    board->board[4][0] = newPiece(KING, WHITE);
    board->board[5][0] = newPiece(BISHOP, WHITE);
    board->board[6][0] = newPiece(KNIGHT, WHITE);
    board->board[7][0] = newPiece(ROCK, WHITE);
    for(int i = 0; i < 8; i++)
        board->board[i][1] = newPiece(PAWN, WHITE);

    for(int i = 0; i < 8; i++)
        board->board[i][6] = newPiece(PAWN, BLACK);
    board->board[0][7] = newPiece(ROCK, BLACK);
    board->board[1][7] = newPiece(KNIGHT, BLACK);
    board->board[2][7] = newPiece(BISHOP, BLACK);
    board->board[3][7] = newPiece(QUEEN, BLACK);
    board->board[4][7] = newPiece(KING, BLACK);
    board->board[5][7] = newPiece(BISHOP, BLACK);
    board->board[6][7] = newPiece(KNIGHT, BLACK);
    board->board[7][7] = newPiece(ROCK, BLACK);
}