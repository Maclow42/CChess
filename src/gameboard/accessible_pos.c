#include "accessible_pos.h"

#include "move_piece.h"
#include "state_backuper.h"


bool isPosAccessible_Pawn(game_board* game_board, coords current_pos, coords end_pos){
    piece*** board = game_board->board;
    int currentx = current_pos.posx, currenty = current_pos.posy;
    int tox = end_pos.posx, toy = end_pos.posy;
    
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

void getAllMoves_Pawn(game_board* board, coords current_pos, list_t* result_list){
    int vect_dir = board->board[current_pos.posx][current_pos.posy]->color == WHITE?1:-1;

    coords dest_pos = {current_pos.posx, current_pos.posy+vect_dir};
    if(isPosAccessible(board, current_pos, dest_pos))
        list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));

    dest_pos = (coords) {current_pos.posx, current_pos.posy+2*vect_dir};
    if(isPosAccessible(board, current_pos, dest_pos))
        list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));
        
    dest_pos = (coords) {current_pos.posx+1, current_pos.posy+vect_dir};
    if(isPosAccessible(board, current_pos, dest_pos))
        list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));
        
    dest_pos = (coords) {current_pos.posx-1, current_pos.posy+vect_dir};
    if(isPosAccessible(board, current_pos, dest_pos))
        list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));
}

bool isPosAccessible_Rock(game_board* game_board, coords current_pos, coords end_pos){
    piece*** board = game_board->board;
    int currentx = current_pos.posx, currenty = current_pos.posy;
    int tox = end_pos.posx, toy = end_pos.posy;
    
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

void getAllMoves_Rock(game_board* board, coords current_pos, list_t* result_list){
    for(int i = 0; i < 8; i++){
        coords dest_pos = {i, current_pos.posy};
        if(isPosAccessible(board, current_pos, dest_pos))
            list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));

        dest_pos = (coords) {current_pos.posx, i};
        if(isPosAccessible(board, current_pos, dest_pos))
            list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));
    }
}

bool isPosAccessible_Knight(game_board* game_board, coords current_pos, coords end_pos){
    int currentx = current_pos.posx, currenty = current_pos.posy;
    int tox = end_pos.posx, toy = end_pos.posy;
    
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

void getAllMoves_Knight(game_board* board, coords current_pos, list_t* result_list){
    for(int i = -2; i <= 2; i++){
        for(int j = -2; j <= 2; j++){
            coords dest_pos = {current_pos.posx+i, current_pos.posy+j};
            if(isPosAccessible(board, current_pos, dest_pos))
                list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));
        }
    }
}

bool isPosAccessible_Bishop(game_board* game_board, coords current_pos, coords end_pos){
    piece*** board = game_board->board;
    int currentx = current_pos.posx, currenty = current_pos.posy;
    int tox = end_pos.posx, toy = end_pos.posy;
    
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

void getAllMoves_Bishop(game_board* board, coords current_pos, list_t* result_list){
    for(int i = 1; i < 8; i++){
        coords dest_pos = {current_pos.posx+i, current_pos.posy+i};
        if(isPosAccessible(board, current_pos, dest_pos))
            list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));

        dest_pos = (coords) {current_pos.posx+i, current_pos.posy-i};
        if(isPosAccessible(board, current_pos, dest_pos))
            list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));

        dest_pos = (coords) {current_pos.posx-i, current_pos.posy+i};
        if(isPosAccessible(board, current_pos, dest_pos))
            list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));

        dest_pos = (coords) {current_pos.posx-i, current_pos.posy-i};
        if(isPosAccessible(board, current_pos, dest_pos))
            list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));
    }
}

bool isPosAccessible_King(game_board* game_board, coords current_pos, coords end_pos){
    piece*** board = game_board->board;
    int currentx = current_pos.posx, currenty = current_pos.posy;
    int tox = end_pos.posx, toy = end_pos.posy;
    
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
            coords current_pos = {tox+i, toy+j};
            if(areCoordsValid(current_pos)){
                if(board[tox+i][toy+j] != NULL && board[tox+i][toy+j]->type == KING)
                    return false;
            }
        }
    }

    return true;    
}

void getAllMoves_King(game_board* board, coords current_pos, list_t* result_list){
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            coords dest_pos = {current_pos.posx+i, current_pos.posy+j};
            if(isPosAccessible(board, current_pos, dest_pos))
                list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));
        }
    }
    coords little_castle = {6, current_pos.posy};
    coords big_castle = {2, current_pos.posy};
    if(isPosAccessible(board, current_pos, little_castle))
        list_rpush(result_list, list_node_new(moveCoords(current_pos, little_castle, 0)));
    if(isPosAccessible(board, current_pos, big_castle))
        list_rpush(result_list, list_node_new(moveCoords(current_pos, big_castle, 0)));
}

bool isPosAccessible_Queen(game_board* game_board, coords current_pos, coords end_pos){
    return  isPosAccessible_Bishop(game_board, current_pos, end_pos) || \
            isPosAccessible_Rock(game_board, current_pos, end_pos);
}

void getAllMoves_Queen(game_board* board, coords current_pos, list_t* result_list){
    for(int i = 0; i < 8; i++){
        coords dest_pos = {current_pos.posx+i, current_pos.posy+i};
        if(isPosAccessible(board, current_pos, dest_pos))
            list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));

        dest_pos = (coords) {current_pos.posx+i, current_pos.posy-i};
        if(isPosAccessible(board, current_pos, dest_pos))
            list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));

        dest_pos = (coords) {current_pos.posx-i, current_pos.posy+i};
        if(isPosAccessible(board, current_pos, dest_pos))
            list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));

        dest_pos = (coords) {current_pos.posx-i, current_pos.posy-i};
        if(isPosAccessible(board, current_pos, dest_pos))
            list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));

        dest_pos = (coords) {i, current_pos.posy};
        if(isPosAccessible(board, current_pos, dest_pos))
            list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));

        dest_pos = (coords) {current_pos.posx, i};
        if(isPosAccessible(board, current_pos, dest_pos))
            list_rpush(result_list, list_node_new(moveCoords(current_pos, dest_pos, 0)));
    }
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
            is_accessible = isPosAccessible_Pawn(board, current_pos, dest_pos);
            break;
        case ROCK:
            is_accessible = isPosAccessible_Rock(board, current_pos, dest_pos);
            break;
        case KNIGHT:
            is_accessible = isPosAccessible_Knight(board, current_pos, dest_pos);
            break;
        case BISHOP:
            is_accessible = isPosAccessible_Bishop(board, current_pos, dest_pos);
            break;
        case QUEEN:
            is_accessible = isPosAccessible_Queen(board, current_pos, dest_pos);
            break;
        case KING:
            is_accessible = isPosAccessible_King(board, current_pos, dest_pos);
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

list_t* getAccessiblePos(game_board* board, enum color color){
    /*
        * Get all possible move for a given color
        * @param board : the game board
        * @param color : the color of the player
    */
    list_t* possible_moves = list_new();

    for(int k = 0; k < 8; k++){
        for(int l = 0; l < 8; l++){
            // if the piece is of the right color
            if(board->board[k][l] != NULL && board->board[k][l]->color == color){
                // get all possible move for this piece
                coords current_pos = {k, l};

                switch (board->board[current_pos.posx][current_pos.posy]->type)
                {
                    case PAWN:
                        getAllMoves_Pawn(board, current_pos, possible_moves);
                        break;
                    case ROCK:
                        getAllMoves_Rock(board, current_pos, possible_moves);
                        break;
                    case KNIGHT:
                        getAllMoves_Knight(board, current_pos, possible_moves);
                        break;
                    case BISHOP:
                        getAllMoves_Bishop(board, current_pos, possible_moves);
                        break;
                    case QUEEN:
                        getAllMoves_Queen(board, current_pos, possible_moves);
                        break;
                    case KING:
                        getAllMoves_King(board, current_pos, possible_moves);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    return possible_moves;
}

bool isInChess(game_board* board, enum color color){
    coords king_pos = color == WHITE
                        ? board->white_king_pos
                        : board->black_king_pos;

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            coords current_pos = {i, j};

            // if no piece at position
            if(board->board[i][j] == NULL || board->board[i][j]->color == color)
                continue;

            bool is_accessible = false;
            switch (board->board[i][j]->type)
            {
                case PAWN:
                    is_accessible = isPosAccessible_Pawn(board, current_pos, king_pos);
                    break;
                case ROCK:
                    is_accessible = isPosAccessible_Rock(board, current_pos, king_pos);
                    break;
                case KNIGHT:
                    is_accessible = isPosAccessible_Knight(board, current_pos, king_pos);
                    break;
                case BISHOP:
                    is_accessible = isPosAccessible_Bishop(board, current_pos, king_pos);
                    break;
                case QUEEN:
                    is_accessible = isPosAccessible_Queen(board, current_pos, king_pos);
                    break;
                case KING:
                    is_accessible = isPosAccessible_King(board, current_pos, king_pos);
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
        fprintf(stderr, "No king found at position (%hi, %hi).\n", kingposx, kingposy);
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
                list_t *access_list = getAccessiblePos(board, color);
                
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