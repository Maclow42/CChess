#pragma once

#include "gameboard.h"

typedef struct{
    coords start_pos;
    coords end_pos;
    piece* taken_piece;
    move_type last_move;
}state_backuper;

void backup_state(game_board* board, coords start_pos, coords end_pos);
void restore_state(game_board* board);