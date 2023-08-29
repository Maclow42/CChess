#include "board.h"

bool isPosAccessible_Pawn(piece*** board, int currentx, int currenty, int tox, int toy){
    // forward move of one case
    if(toy == currenty + (board[currentx][currenty]->color == WHITE?1:-1)){
        if(tox == currentx)
            return board[tox][toy] == NULL;
        // move diagonaly to catch a piece
        else if(abs(tox - currentx) == 1)
            return board[tox][toy] != NULL;
        
        return false;
    }

    // if it's the first move of the pawn, it can move of 2 cases
    if(board[currentx][currenty]->first_move){
        if(board[currentx][currenty]->color == WHITE){
            if(toy == currenty + 2 && tox == currentx){
                    return board[tox][toy] == NULL;
            }
        }
        else{
            if(toy == currenty - 2 && tox == currentx){
                    return board[tox][toy] == NULL;
            }
        }
    }

    return false;
}

bool isPosAccessible_Rock(piece*** board, int currentx, int currenty, int tox, int toy){
    if(toy != currenty && tox != currentx)
        return false;
    
    // if the asked deplacement is vertical
    if(tox == currentx){
        for(int i = currenty+(toy>currenty?1:-1); i != toy; toy>currenty?i++:i--){
            if(board[currentx][i] != NULL)
                return false;
        }
    }

    // else the deplacement is horizontal
    else{
        for(int i = currentx+(tox>currentx?1:-1); i != tox; tox>currentx?i++:i--){
            if(board[i][currenty] != NULL)
                return false;
        }
    }

    return true;
}

bool isPosAccessible_Knight(piece*** board, int currentx, int currenty, int tox, int toy){
    // Array listing all relative positions accessible by a knight
    int relative_accessible[8][2] = {  {-2, 1},
                                        {-1, 2},
                                        {1, 2},
                                        {2, 1},
                                        {2, -1},
                                        {1, -2},
                                        {-1, -2},
                                        {-2, -1}
                                     };
    // check if one possibility corresponds to the asked deplacement
    for(int i = 0; i < 8; i++){
        if(currentx + relative_accessible[i][0] == tox && currenty + relative_accessible[i][1] == toy)
            return true;
    }

    return false;
}

bool isPosAccessible_Bishop(piece*** board, int currentx, int currenty, int tox, int toy){
    if(abs(currentx - tox) != abs(currenty - toy))
        return false;

    int dist = abs(currentx - tox);

    int vect_x = abs(tox - currentx)/(tox - currentx);
    int vect_y = abs(toy - currenty)/(toy - currenty);

    for(int i = 1; i < dist; i++){
        if(board[currentx + vect_x*i][currenty + vect_y*i] != NULL)
            return false;
    }

    return true;
}

bool isPosAccessible_King(piece*** board, int currentx, int currenty, int tox, int toy){
    if(abs(currentx - tox) > 1 || abs(currenty - toy) > 1)
        return false;

    return true;    
}

bool isPosAccessible_Queen(piece*** board, int currentx, int currenty, int tox, int toy){
    return  isPosAccessible_Bishop(board, currentx, currenty, tox, toy) || \
            isPosAccessible_Rock(board, currentx, currenty, tox, toy);
}

bool isPosAccessible(game_board* board, coords current_pos, coords dest_pos){
    /*
    Return true if piece at (currentx,currenty) in board can be moved to (tox, toy), otherwise return false
    Arguments:
    */

    unsigned int currentx = current_pos.posx;
    unsigned int currenty = current_pos.posy;

    unsigned int tox = dest_pos.posx;
    unsigned int toy = dest_pos.posy;

    // if coords are not valid then return false
    if(!(areCoordsValid(&current_pos) && areCoordsValid(&dest_pos))){
        //printf("(%i, %i) to (%i, %i) => invalid coords\n", current_pos->posx, current_pos->posy, dest_pos->posx, dest_pos->posy);
        return false;
    }

    // check if there is no piece at the current position
    if(board->board[currentx][currenty] == NULL){
        //printf("(%i, %i) to (%i, %i) => NULL piece to move\n", current_pos->posx, current_pos->posy, dest_pos->posx, dest_pos->posy);
        return false;
    }

    // check if the arrival position is not occupied by a piece of the same color
    if(board->board[tox][toy] != NULL && board->board[tox][toy]->color == board->board[currentx][currenty]->color){
        //printf("(%i, %i) to (%i, %i) => trying to bit an allie\n", current_pos->posx, current_pos->posy, dest_pos->posx, dest_pos->posy);
        return false;
    }

    bool is_accessible = false;
    switch (board->board[currentx][currenty]->type)
    {
        case PAWN:
            is_accessible = isPosAccessible_Pawn(board->board, currentx, currenty, tox, toy);
            break;
        case ROCK:
            is_accessible = isPosAccessible_Rock(board->board, currentx, currenty, tox, toy);
            break;
        case KNIGHT:
            is_accessible = isPosAccessible_Knight(board->board, currentx, currenty, tox, toy);
            break;
        case BISHOP:
            is_accessible = isPosAccessible_Bishop(board->board, currentx, currenty, tox, toy);
            break;
        case QUEEN:
            is_accessible = isPosAccessible_Queen(board->board, currentx, currenty, tox, toy);
            break;
        case KING:
            is_accessible = isPosAccessible_King(board->board, currentx, currenty, tox, toy);
            break;
        default:
            is_accessible = false;
            break;
    }

    if(!is_accessible)
        return false;

    //try to move the piece and see if the king is in chess or mate
    color_t current_color = board->board[currentx][currenty]->color;
    piece* tmp = board->board[tox][toy];
    movePiece(board, current_pos, dest_pos);
    bool is_chess = isInChess(board, current_color == WHITE ? board->white_king_pos : board->black_king_pos);
    movePiece(board, dest_pos, current_pos);
    board->board[tox][toy] = tmp;

    return !is_chess;
}

void movePiece(game_board* board, coords current_pos, coords dest_pos){
    int currentx = current_pos.posx, currenty = current_pos.posy;
    int tox = dest_pos.posx, toy = dest_pos.posy;

    board->board[tox][toy] = board->board[currentx][currenty];
    board->board[currentx][currenty] = NULL;

    if(board->board[tox][toy] != NULL && board->board[tox][toy]->type == KING){
        if(board->board[tox][toy]->color == WHITE){
            board->white_king_pos = Coords(dest_pos.posx, dest_pos.posy);
        }
        else{
            board->black_king_pos = Coords(dest_pos.posx, dest_pos.posy);
        }
    }
}

bool playerMovePiece(game_board* board, coords current_pos, coords dest_pos){
    if(board->board[current_pos.posx][current_pos.posy] == NULL){
        printf("No piece at the given position.\n");
        return false;
    }

    if(board->board[current_pos.posx][current_pos.posy]->color != board->to_play){
        printf("It's not your turn to play.\n");
        return false;
    }

    if(!isPosAccessible(board, current_pos, dest_pos)){
        printf("The piece can't be moved to the given position.\n");
        return false;
    }

    movePiece(board, current_pos, dest_pos);
    board->last_played = board->board[dest_pos.posx][dest_pos.posy];
    board->board[dest_pos.posx][dest_pos.posy]->first_move = false;
    board->to_play = board->to_play == WHITE ? BLACK : WHITE;

    return true;
}


list_t* getPossiblePos(game_board* board, coords start_pos){
    list_t *result_list = list_new();

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++){
            coords *current_ij = malloc(sizeof(coords));
            current_ij->posx = i;
            current_ij->posy = j;
            if(isPosAccessible(board, start_pos, *current_ij)){
                list_rpush(result_list, list_node_new(current_ij));
            }
        }
    
    return result_list;
}

int isInChess(game_board* board, coords kingpos){
    int kingposx = kingpos.posx, kingposy = kingpos.posy;

    if(board->board[kingposx][kingposy] == NULL || board->board[kingposx][kingposy]->type != KING){
        //fprintf(stderr, "No king found at position (%hi, %hi).\n", kingposx, kingposy);
        return false;
    }

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            // check if enemies can eat the king
            coords current_pos = {i, j};
            bool is_accessible = isPosAccessible(board, current_pos, kingpos);
            
            if(is_accessible){
                return true;
            }
        }
    }

    return false;
}

bool isMate(game_board* board, coords kingpos){
    // check if the king can move out of chess
    // Array listing all relative positions accessible by the king

    int kingposx = kingpos.posx, kingposy = kingpos.posy;

    if(board->board[kingposx][kingposy] == NULL || board->board[kingposx][kingposy]->type != KING){
        //fprintf(stderr, "No king found at position (%hi, %hi).\n", kingposx, kingposy);
        return false;
    }

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
        coords dest_pos = {kingposx+relative_accessible[i][0], kingposy+relative_accessible[i][1]};
        if(areCoordsValid(&dest_pos)){
            // if the king can move at the position
            piece* tmp = board->board[dest_pos.posx][dest_pos.posy];
            if(isPosAccessible(board, kingpos, dest_pos)){
                movePiece(board, kingpos, dest_pos);
                bool isChess = isInChess(board, dest_pos);
                movePiece(board, dest_pos, kingpos);
                board->board[dest_pos.posx][dest_pos.posy] = tmp;
                if(!isChess){
                    return false;
                }
            }
        }
    }

    // check if a piece can eat or cover the ennemy
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){

            // if no piece there.... just try the next piece
            if(board->board[i][j] == NULL)
                continue;

            // if the current piece belongs to the player (same color as the current king)
            if(board->board[i][j]->color == board->board[kingposx][kingposy]->color){
                // if it's the king himself... go to next
                if(i == kingposx && j == kingposy)
                    continue;

                // get all accessible positions
                coords current_pos = Coords(i, j);
                list_t *access_list = getPossiblePos(board, current_pos);

                #if 0
                // move the current piece to all accessible positions
                bool is_chess = true;
                list_node_t *node;
                list_iterator_t *it = list_iterator_new(access_list, LIST_HEAD);
                while ((node = list_iterator_next(it)) && is_chess) {
                    coords dest_pos = *(coords*)node->val;
                    piece* tmp = board->board[dest_pos.posx][dest_pos.posy];
                    // try to move the piece
                    if(isPosAccessible(board, current_pos, dest_pos)){
                        return false;
                    }
                    else
                        printf("strange that (%i, %i) to (%i, %i) not possible\n", i, j, dest_pos.posx, dest_pos.posy);
                }

                list_iterator_destroy(it);
                #endif

                if(access_list->len != 0){
                    list_destroy(access_list);
                    return false;
                }
                list_destroy(access_list);

            }
        }
    }
    return true;

}

game_status getGameStatus(game_board* board)
{
    bool white_chess = isInChess(board, board->white_king_pos);
    bool white_mate = isMate(board, board->white_king_pos);

    if(white_chess){
        if(white_mate)
            return WHITE_MATE;
        return WHITE_CHESS;
    }
    if(white_mate)
        return PAT;

    bool black_chess = isInChess(board, board->black_king_pos);
    bool black_mate = isMate(board, board->black_king_pos);

    if(black_chess){
        if(black_mate)
            return BLACK_MATE;
        return BLACK_CHESS;
    }
    if(black_mate)
        return PAT;

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
            if(board->board[i][j] == NULL)
                printf("%s", "   ");
            else
                switch (board->board[i][j]->type){
                    case PAWN:
                        printf("%s", board->board[i][j]->color == BLACK ? " ♙ " : " ♟ ");
                        break;
                    case ROCK:
                        printf("%s", board->board[i][j]->color == BLACK ? " ♖ " : " ♜ ");
                        break;
                    case KNIGHT:
                        printf("%s", board->board[i][j]->color == BLACK ? " ♘ " : " ♞ ");
                        break;
                    case BISHOP:
                        printf("%s", board->board[i][j]->color == BLACK ? " ♗ " : " ♝ ");
                        break;
                    case QUEEN:
                        printf("%s", board->board[i][j]->color == BLACK ? " ♕ " : " ♛ ");
                        break;
                    case KING:
                        printf("%s", board->board[i][j]->color == BLACK ? " ♔ " : " ♚ ");
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
    board->white_king_pos = Coords(4, 0);
    board->black_king_pos = Coords(4, 7);

    board->to_play = WHITE;

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