#include "board.h"
#include "src/list/list.h"

bool isPosAccessible_Pawn(piece board[8][8], int currentx, int currenty, int tox, int toy){
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

bool isPosAccessible_Rock(piece board[8][8], int currentx, int currenty, int tox, int toy){
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

bool isAccessible_Knight(piece board[8][8], int currentx, int currenty, int tox, int toy){
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

bool isAccessible_Bishop(piece board[8][8], int currentx, int currenty, int tox, int toy){
    if(abs(currentx - tox) != abs(currenty - toy))
        return false;

    int dist = abs(currentx - tox);

    int vect_x = abs(tox - currentx)/(tox - currentx);
    int vect_y = abs(toy - currenty)/(toy - currenty);

    for(int i = 1; i < dist; i++){
        if(board[currentx + vect_x*i][currenty + vect_y*i].color != NONE_PIECE)
            return false;
    }

    return true;
}

bool isAccessible_King(piece board[8][8], int currentx, int currenty, int tox, int toy){
    if(abs(currentx - tox) > 1 || abs(currenty - toy) > 1)
        return false;

    return true;    
}

bool isAccessible_Queen(piece board[8][8], int currentx, int currenty, int tox, int toy){
    return  isAccessible_Bishop(board, currentx, currenty, tox, toy) || \
            isPosAccessible_Rock(board, currentx, currenty, tox, toy);
}

bool isPosAccessible(piece board[8][8], int currentx, int currenty, int tox, int toy){
    /*
    Return true if piece at (currentx,currenty) in board can be moved to (tox, toy), otherwise return false
    Arguments:
        piece board[8][8] : piece array representing game board
        int currentx : horizontal position of the piece
        int currenty : vertical position of the piece
        int tox : horizontal position of the arrival
        int toy : vertical position of the arrival
    */

    // check if positions are valids
    if(currentx < 0 || currenty < 0 || currentx >= 8 || currenty >= 8)
        return false;
    if(tox < 0 || toy < 0 || tox >= 8 || toy >= 8)
        return false;

    // check if there actually is a piece at the indicated current position (if not return false)
    if(board[currentx][currenty].type == NONE_PIECE)
        return false;
    
    // check if the arrival position is not occupied by a piece of the same color
    if(board[tox][toy].color == board[currentx][currenty].color)
        return false;

    int (*functionTable[6])(int board[8][8], int currentx, int currenty, int tox, int toy) = { isPosAccessible_Pawn,
                                                                                                isPosAccessible_Rock,
                                                                                                isAccessible_Knight,
                                                                                                isAccessible_Bishop,
                                                                                                isAccessible_Queen,
                                                                                                isAccessible_King};
    return functionTable[board[currentx][currenty].type-1](board, currentx, currenty, tox, toy);
}

bool isPosAccessible_noPositionCheck(piece board[8][8], int currentx, int currenty, int tox, int toy){
    /*
        Same as isPosAccessible but do not check if indicated positions are valids
        (private function, used chen position already checked)
    */

    // check if there actually is a piece at the indicated current position (if not return false)
    if(board[currentx][currenty].type == NONE_PIECE)
        return false;
    
    // check if the arrival position is not occupied by a piece of the same color
    if(board[tox][toy].color == board[currentx][currenty].color)
        return false;

    int (*functionTable[6])(int board[8][8], int currentx, int currenty, int tox, int toy) = { isPosAccessible_Pawn,
                                                                                                    isPosAccessible_Rock,
                                                                                                    isAccessible_Knight,
                                                                                                    isAccessible_Bishop,
                                                                                                    isAccessible_Queen,
                                                                                                    isAccessible_King};
    return functionTable[board[currentx][currenty].type-1](board, currentx, currenty, tox, toy);
}

bool movePiece(piece board[8][8], int currentx, int currenty, int tox, int toy){
    if(isPosAccessible(board, currentx, currenty, tox, toy)){
        board[tox][toy] = board[currentx][currenty];
        board[currentx][currenty] = newPiece(NONE_PIECE, NONE_COLOR);
        return true;
    }

    return false;
}

list_t* getPossiblePos(piece board[8][8], int currentx, int currenty){
    list_t *result = newList();

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            if(isPosAccessible(board, currentx, currenty, i, j))
                pushList(result, (int*)(100 + 10*i+j));
    
    return result;
}

int isInChess(piece board[8][8], int kingposx, int kingposy){
    if(board[kingposx][kingposy].type != KING){
        fprintf(stderr, "No king found at position (%hi, %hi).\n", kingposx, kingposy);
        return false;
    }

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(isPosAccessible_noPositionCheck(board, i, j, kingposx, kingposy))
                return 100 + 10*kingposx + kingposy;
        }
    }

    return false;
}

bool isMate(piece board[8][8], int kingposx, int kingposy){
    // check if the king can move out of chess
    // Array listing all relative positions accessible by the king
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
        int tox = kingposx+relative_accessible[i][0];
        int toy = kingposy+relative_accessible[i][1];
        // if the king can move at the position
        piece tmp = board[tox][toy];
        if(movePiece(board, kingposx, kingposy, tox, toy)){
            bool isChess = isInChess(board, tox, toy);
            movePiece(board, tox, toy, kingposx, kingposy);
            board[tox][toy] = tmp;
            if(!isChess)
                return false;
        }
    }

    // check if a piece can eat or cover the ennemy
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(board[i][j].color == board[kingposx][kingposy].color && 10*i+j!= 10*kingposx+kingposy){
                // get all accessible positions
                list_t *access = getPossiblePos(board, i, j);
                // move the current piece to all accessible positions
                while(access->size!=0){
                    int to_pos = getIndexList(access, 0);
                    popList(access, 0);
                    to_pos %= 100;
                    int tox = to_pos/10,  toy = to_pos%10;
                    piece tmp = board[tox][toy];
                    movePiece(board, i, j, tox, toy);
                    bool is_chess = isInChess(board, kingposx, kingposy);
                    movePiece(board, tox, toy, i, j);
                    board[tox][toy] = tmp;
                    if(!is_chess)
                        return false;
                }
            }
        }
    }

    return true;

}

void printBoard(piece board[8][8]){

    printf("     A   B   C   D   E   F   G   H\n");
    printf("   ┌───┬───┬───┬───┬───┬───┬───┬───┐\n");
    for(int j = 7; j >= 0; j--){
        if(j != 7)
            printf("   ├───┼───┼───┼───┼───┼───┼───┼───┤\n");
        printf(" %i %s", j+1, "│");
        for(int i = 0; i < 8; i++){
            switch (board[i][j].type){
                case PAWN:
                    printf("%s", board[i][j].color == BLACK ? " ♙ " : " ♟ ");
                    break;
                case ROCK:
                    printf("%s", board[i][j].color == BLACK ? " ♖ " : " ♜ ");
                    break;
                case KNIGHT:
                    printf("%s", board[i][j].color == BLACK ? " ♘ " : " ♞ ");
                    break;
                case BISHOP:
                    printf("%s", board[i][j].color == BLACK ? " ♗ " : " ♝ ");
                    break;
                case QUEEN:
                    printf("%s", board[i][j].color == BLACK ? " ♕ " : " ♛ ");
                    break;
                case KING:
                    printf("%s", board[i][j].color == BLACK ? " ♔ " : " ♚ ");
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

void newBoard(piece board[8][8]){
    board[0][0] = newPiece(ROCK, WHITE);
    board[1][0] = newPiece(KNIGHT, WHITE);
    board[2][0] = newPiece(BISHOP, WHITE);
    board[3][0] = newPiece(QUEEN, WHITE);
    board[4][0] = newPiece(KING, WHITE);
    board[5][0] = newPiece(BISHOP, WHITE);
    board[6][0] = newPiece(KNIGHT, WHITE);
    board[7][0] = newPiece(ROCK, WHITE);
    for(int i = 0; i < 8; i++)
        board[i][1] = newPiece(PAWN, WHITE);

    for(int j = 2; j <= 6; j++){
        for(int i = 0; i < 8; i++){
            board[i][j] = newPiece(NONE_PIECE, NONE_COLOR);
        }
    }

    for(int i = 0; i < 8; i++)
        board[i][6] = newPiece(PAWN, BLACK);
    board[0][7] = newPiece(ROCK, BLACK);
    board[1][7] = newPiece(KNIGHT, BLACK);
    board[2][7] = newPiece(BISHOP, BLACK);
    board[3][7] = newPiece(QUEEN, BLACK);
    board[4][7] = newPiece(KING, BLACK);
    board[5][7] = newPiece(BISHOP, BLACK);
    board[6][7] = newPiece(KNIGHT, BLACK);
    board[7][7] = newPiece(ROCK, BLACK);
}