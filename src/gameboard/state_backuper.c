#include "state_backuper.h"
#include "move_piece.h"

void backup_state(game_board* board, coords current_pos, coords end_pos){
    /*
        * Backup the state of the board
        * @param board : the game board
        * @param current_pos : the starting position of the piece
        * @param end_pos : the ending position of the piece
    */
    state_backuper* to_save = malloc(sizeof(state_backuper));
    to_save->start_pos = current_pos;
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
