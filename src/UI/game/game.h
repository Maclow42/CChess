#pragma once

#include <ncurses.h>
#include "../../gameboard/gameboard.h"

int game_UI(WINDOW* boite, int nb_player, color_t player_color);