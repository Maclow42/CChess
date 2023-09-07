#include "board.h"

#define DP printf("%s : %d\n", __FILE__, __LINE__);

void backup_state(game_board* board, coords start_pos, coords end_pos){
    /*
        * Backup the state of the board
        * @param board : the game board
        * @param start_pos : the starting position of the piece
        * @param end_pos : the ending position of the piece
    */
    state_backuper* to_save = malloc(sizeof(state_backuper));
    to_save->start_pos = start_pos;
    to_save->end_pos = end_pos;
    to_save->taken_piece = board->board[end_pos.posx][end_pos.posy];
    to_save->last_move = board->last_move_type;

    stack_push(board->moves_stack, to_save);
}

void restore_state(game_board* board){
    /*
        * Restore the state of the board
        * @param board : the game board
    */
    state_backuper* to_restore = stack_pop(board->moves_stack);

    switch (board->last_move_type)
    {
    case PAWN_PROMOTION:
        board->board[to_restore->end_pos.posx][to_restore->end_pos.posy]->type = PAWN;
        board->board[to_restore->end_pos.posx][to_restore->end_pos.posy]->value = 1;
        break;
    case LITTLE_CASTLE:
        board->board[7][to_restore->end_pos.posy] = board->board[5][to_restore->end_pos.posy];
        board->board[5][to_restore->end_pos.posy] = NULL;
        break;
    case BIG_CASTLE:
        board->board[0][to_restore->end_pos.posy] = board->board[3][to_restore->end_pos.posy];
        board->board[3][to_restore->end_pos.posy] = NULL;
        break;
    default:
        break;
    }

    movePiece(board, to_restore->end_pos, to_restore->start_pos);

    board->board[to_restore->end_pos.posx][to_restore->end_pos.posy] = to_restore->taken_piece;

    board->last_move_type = to_restore->last_move;

    free(to_restore);
}

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
                    return board[tox][toy-1] == NULL && board[tox][toy] == NULL;
            }
        }
        else{
            if(toy == currenty - 2 && tox == currentx){
                    return board[tox][toy+1] == NULL && board[tox][toy] == NULL;
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
        int vect_dir = (toy>currenty?1:-1);
        for(int i = currenty+vect_dir; i != toy; i+=vect_dir){
            if(board[currentx][i] != NULL)
                return false;
        }
    }

    // else the deplacement is horizontal
    else{
        int vect_dir = (tox>currentx?1:-1);
        for(int i = currentx+vect_dir; i != tox; i+=vect_dir){
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

    int vect_x = abs(tox - currentx)/(tox - currentx); // no risk of division by 0 (tested in isPosAccessible)
    int vect_y = abs(toy - currenty)/(toy - currenty);

    for(int i = 1; i < dist; i++){
        if(board[currentx + vect_x*i][currenty + vect_y*i] != NULL)
            return false;
    }

    return true;
}


bool isPosAccessible_King(piece*** board, int currentx, int currenty, int tox, int toy){
    // if this is a castle move

    if(abs(currentx - tox) > 1){
        // Castle ?
        // if the king is in his initial position and the movement is horizontal
        if(currenty == toy && board[currentx][currenty]->first_move){
            // test if big castle
            if(tox == 2){
                if(board[1][currenty] == NULL && board[3][currenty] == NULL)
                    return board[0][currenty] != NULL && board[0][currenty]->first_move;
                return false;
            }
            // test if little castle
            else if(tox == 6){
                if(board[5][currenty] == NULL) // no need to check if board[6][currenty] is NULL
                    return board[7][currenty] != NULL && board[7][currenty]->first_move;
                return false;
            }
            return false;
        }

        return false;
    }

    if(abs(currenty - toy) > 1)
        return false;


    // if a king of another color is in cases around dest_pos
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(tox+i == currentx && toy+j == currenty)
                continue;
            coords current_pos = Coords(tox+i, toy+j);
            if(areCoordsValid(current_pos)){
                if(board[tox+i][toy+j] != NULL && board[tox+i][toy+j]->type == KING)
                    return false;
            }
        }
    }

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
    if(!(areCoordsValid(current_pos) && areCoordsValid(dest_pos))){
        //printf("(%i, %i) to (%i, %i) => invalid coords\n", current_pos->posx, current_pos->posy, dest_pos->posx, dest_pos->posy);
        return false;
    }

    // check if there is no piece at the current position
    if(board->board[currentx][currenty] == NULL){
        //printf("(%i, %i) to (%i, %i) => NULL piece to move\n", current_pos->posx, current_pos->posy, dest_pos->posx, dest_pos->posy);
        return false;
    }

    if(board->board[tox][toy] != NULL){
        // check if the arrival position is not occupied by a piece of the same color
        if(board->board[tox][toy]->color == board->board[currentx][currenty]->color)
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

    backup_state(board, current_pos, dest_pos);

    movePiece(board, current_pos, dest_pos);
    bool is_chess = isInChess(board, current_color);

    restore_state(board);

    return !is_chess;
}

piece* movePiece(game_board* board, coords current_pos, coords dest_pos){
    /*
        * Move the piece at current_pos to dest_pos
        * Return the taken piece (NULL if no piece at dest_pos)
        * Update board->last_move_type
            * PAWN_PROMOTION if a pawn is promoted
            * LITTLE_CASTLE if a little castle is done
            * BIG_CASTLE if a big castle is done
            * NORMAL_MOVE otherwise
        * Update the king position if one is moved
        * @param board : the game board
        * @param current_pos : the current position of the piece
        * @param dest_pos : the destination position of the piece
    */
   
    // if no piece to move -> return NULL
    // verification useless because movePiece always used after isPosAccessible

    int currentx = current_pos.posx, currenty = current_pos.posy;
    int tox = dest_pos.posx, toy = dest_pos.posy;

    // returned the taken piece (NULL if no piece at dest_pos)
    piece* taken = board->board[tox][toy];

    board->board[tox][toy] = board->board[currentx][currenty];
    board->board[currentx][currenty] = NULL;
    board->last_move_type = NORMAL_MOVE;

    // update king position if needed
    if(board->board[tox][toy]->type == KING){
        // if it was a castle move
        if(currentx == 4 && tox == 2){
            board->board[3][currenty] = board->board[0][currenty];
            board->board[0][currenty] = NULL;
            board->last_move_type = BIG_CASTLE;
        }
        else if(currentx == 4 && tox == 6){
            board->board[5][currenty] = board->board[7][currenty];
            board->board[7][currenty] = NULL;
            board->last_move_type = LITTLE_CASTLE;
        }
        if(board->board[tox][toy]->color == WHITE){
            board->white_king_pos.posx = tox;
            board->white_king_pos.posy = toy;
        }
        else{
            board->black_king_pos.posx = tox;
            board->black_king_pos.posy = toy;
        }
    }

    //promote pawn if needed
    else if(board->board[tox][toy]->type == PAWN){
        if(board->board[tox][toy]->color == WHITE && toy == 7){
            board->board[tox][toy]->type = QUEEN;
            board->board[tox][toy]->value = 15;
            board->last_move_type = PAWN_PROMOTION;
        }
        else if(board->board[tox][toy]->color == BLACK && toy == 0){
            board->board[tox][toy]->type = QUEEN;
            board->board[tox][toy]->value = 15;
            board->last_move_type = PAWN_PROMOTION;
        }
    }

    return taken;
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

    piece* taken = movePiece(board, current_pos, dest_pos);

    if(board->last_move_type == LITTLE_CASTLE || board->last_move_type == BIG_CASTLE){
        if(board->to_play == WHITE)
            board->white_castled = true;
        else
            board->black_castled = true;
    }

    if(taken != NULL){
        board->nb_piece--;
        free(taken);
    }

    board->last_played = board->board[dest_pos.posx][dest_pos.posy];
    board->board[dest_pos.posx][dest_pos.posy]->first_move = false;
    board->to_play = board->to_play == WHITE ? BLACK : WHITE;

    return true;
}

list_t* getPossiblePos(game_board* board, coords start_pos){
    list_t *result_list = list_new();
    result_list->free = free;

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

bool isInChess(game_board* board, enum color color){
    int kingposx, kingposy;
    if(color == WHITE){
        kingposx = board->white_king_pos.posx;
        kingposy = board->white_king_pos.posy;
    }else{
        kingposx = board->black_king_pos.posx;
        kingposy = board->black_king_pos.posy;
    }

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            // if no piece at position
            if(board->board[i][j] == NULL || board->board[i][j]->color == color)
                continue;

            bool is_accessible = false;
            switch (board->board[i][j]->type)
            {
                case PAWN:
                    is_accessible = isPosAccessible_Pawn(board->board, i, j, kingposx, kingposy);
                    break;
                case ROCK:
                    is_accessible = isPosAccessible_Rock(board->board, i, j, kingposx, kingposy);
                    break;
                case KNIGHT:
                    is_accessible = isPosAccessible_Knight(board->board, i, j, kingposx, kingposy);
                    break;
                case BISHOP:
                    is_accessible = isPosAccessible_Bishop(board->board, i, j, kingposx, kingposy);
                    break;
                case QUEEN:
                    is_accessible = isPosAccessible_Queen(board->board, i, j, kingposx, kingposy);
                    break;
                case KING:
                    is_accessible = isPosAccessible_King(board->board, i, j, kingposx, kingposy);
                    break;
                default:
                    is_accessible = false;
                    break;
            }

            if(is_accessible)
                return true;

        }
    }

    return false;

}

bool isMate(game_board* board, enum color color){
    // check if the king can move out of chess
    // Array listing all relative positions accessible by the king

    coords kingpos;
    int kingposx, kingposy;
    if(color == WHITE){
        kingpos = board->white_king_pos;
        kingposx = board->white_king_pos.posx;
        kingposy = board->white_king_pos.posy;
    }else{
        kingpos = board->black_king_pos;
        kingposx = board->black_king_pos.posx;
        kingposy = board->black_king_pos.posy;
    }

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
        if(areCoordsValid(dest_pos)){
            // if the king can move at the position
            if(isPosAccessible(board, kingpos, dest_pos)){
                backup_state(board, kingpos, dest_pos);

                movePiece(board, kingpos, dest_pos);
                bool isChess = isInChess(board, color);

                restore_state(board);

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

#define NUM_THREADS 4
game_status getGameStatus(game_board* board)
{
    if(board->nb_piece == 2)
        return PAT;

    bool white_chess = isInChess(board, WHITE);
    bool white_mate = isMate(board, WHITE);

    if(white_chess){
        if(white_mate)
            return WHITE_MATE;
        return WHITE_CHESS;
    }
    if(white_mate)
        return PAT;

    bool black_chess = isInChess(board, BLACK);
    bool black_mate = isMate(board, BLACK);

    if(black_chess){
        if(black_mate)
            return BLACK_MATE;
        return BLACK_CHESS;
    }
    if(black_mate)
        return PAT;

    return RAS;
}

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

    board->white_king_pos = Coords(4, 0);
    board->black_king_pos = Coords(4, 7);

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