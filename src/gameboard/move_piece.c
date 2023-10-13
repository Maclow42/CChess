#include "move_piece.h"

#include "accessible_pos.h"


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