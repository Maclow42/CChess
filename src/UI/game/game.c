#include "game.h"

#include <ncurses.h>
#include <wchar.h>
#include "../../gameboard/gameboard.h"
#include "../../gameboard/move_piece.h"
#include "../../gameboard/game_status.h"
#include "../../minmax/minmax.h"

#define WINDOW_HEIGHT 19
#define WINDOW_WIDTH 37

game_status game_board_play_AI(game_board *game_board){
    movement_coords* best_move = getBestMove(game_board, 3);
    coords start_pos = best_move->start_pos;
    coords end_pos = best_move->end_pos;

    if(playerMovePiece(game_board, start_pos, end_pos) == OK_DONE){
        return getGameStatus(game_board);
    }

    attron(COLOR_PAIR(-1));
    mvprintw(0, 0, "Error : AI tried to play an invalid move\n");
    attroff(COLOR_PAIR(-1));
    return -1;
}

void reset_to_NULL_Coords(coords** c){
    coords *tmp = *c;
    *c = NULL;
    free(tmp);
}

void refresh_display(WINDOW* window, game_board* game_board, coords* selected_piece){
    // Allocate memory to stock the result[][] of printBoard
    char** result = (char**)malloc(20 * sizeof(char*));
    for (int i = 0; i < 20; ++i) {
        result[i] = (char*)malloc(102 * sizeof(char));
    }

    // Get the new string of board to display
    printBoard(game_board, BLACK, result);
    // Print the board
    wattron(window, COLOR_PAIR(1));
    for (int i = 0; i < 20; i++) {
        mvwprintw(window, i, 0, "%s", result[i]);
    }
    mvprintw(1, 1, "Color to play : %s", game_board->to_play == WHITE ? "WHITE" : "BLACK");
    mvprintw(LINES - 2, 1, "Press 'q' to quit");
    wattroff(window, COLOR_PAIR(2));

    char piece[6] = "NULL";
    // If the mouse is on a piece, replace it with a colored one
    if(selected_piece){
        print_piece(piece, game_board->board[selected_piece->posx][selected_piece->posy]);
        wattron(window, COLOR_PAIR(2));
        mvwaddstr(window, selected_piece->posy*2+2, selected_piece->posx*4+3, piece);
        wattroff(window, COLOR_PAIR(2));
    }

    wrefresh(window);
    refresh();

    // free the memory allocated to result
    for (int i = 0; i < 20; ++i) {
        free(result[i]);
    }
    free(result);
}

int game_UI(WINDOW* window, int nb_player, color_t playerColor){
    // Init game board
    game_board *game_board = newBoard();
    initGameBoard(game_board);

    // Init coordinates used to move pieces
    coords *start_pos = NULL;
    coords *end_pos = NULL;

    // Resize window and move it to center
    wresize(window, WINDOW_HEIGHT, WINDOW_WIDTH);

    // Init ncurses colors
    init_pair(-1, COLOR_RED, COLOR_BLACK); // print red on black
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // print white on black
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // print black on white
    
    // Main loop
    int ch; // char read by wgetch
    MEVENT event; // mouse event
    do{
        // window's position to center it
        int start_y = (LINES - WINDOW_HEIGHT) / 2;
        int start_x = (COLS - WINDOW_WIDTH) / 2;
        mvwin(window, start_y, start_x);

        // If start_pos and end_pos are set, try to move the piece
        if(start_pos && end_pos){
            switch(playerMovePiece(game_board, *start_pos, *end_pos))
            {
                case ERROR_INVALID_MOVE:
                    attron(COLOR_PAIR(-1));
                    mvprintw(2, 1, "Error : the piece can't be moved to the given position");
                    attroff(COLOR_PAIR(-1));
                    break;
                case ERROR_CHESS:
                    attron(COLOR_PAIR(-1));
                    mvprintw(2, 1, "Error : the piece can't be moved to the given position because it would put the king in check");
                    attroff(COLOR_PAIR(-1));
                    break;
                case OK_DONE:
                    if (nb_player == 1){
                        refresh_display(window, game_board, NULL);
                        mvprintw(1, 1, "IA is playing...     ");
                        refresh();
                        game_board_play_AI(game_board);
                    }
                default:
                    break;
            }
            // Reset start_pos and end_pos to NULL
            reset_to_NULL_Coords(&start_pos);
            reset_to_NULL_Coords(&end_pos);
        }

        // If mouse click event => get relative coordinates with wmouse_trafo
        if (ch && ch == KEY_MOUSE) {
            if (getmouse(&event) == OK && event.bstate & BUTTON1_CLICKED) {
                // If mouse click is outside the board
                if(event.y < start_y + 2 || event.y >= start_y + WINDOW_HEIGHT - 1 || event.x < start_x + 3 || event.x >= start_x + WINDOW_WIDTH - 2)
                    event.y = -1, event.x = -1;
                else
                    wmouse_trafo(window, &event.y, &event.x, FALSE);
            }
        }

        // Get the piece pointed by the mouse by working on the relative coordinates
        // If click on the board, process click (exclude line_id and letters)
        if(event.x > 0 && event.x <= WINDOW_WIDTH-5 && event.y > 0 && event.y <= WINDOW_HEIGHT-3){
            int posx = event.x/4 - 1, posy = event.y/2 - 1;
            // Save its coordinates
            if(!start_pos){
                if(game_board->board[posx][posy] && game_board->board[posx][posy]->color == game_board->to_play)
                    start_pos = Coords(posx, posy);
                else
                    start_pos = NULL;
            }
            else{
                if(areCoordsEqual((*start_pos), ((coords){posx, posy})))
                    reset_to_NULL_Coords(&start_pos);
                else if(game_board->board[posx][posy] && game_board->board[posx][posy]->color == game_board->to_play)
                    start_pos = Coords(posx, posy);
                else
                    end_pos = Coords(posx, posy);
            }
        }
        else{ // if click outside the board, reset start_pos and end_pos
            reset_to_NULL_Coords(&start_pos);
            reset_to_NULL_Coords(&end_pos);
        }

        refresh_display(window, game_board, start_pos);

    }while(end_pos || (ch = wgetch(window)) != 'q'); // Use 'q' to quit

    free(start_pos);
    free(end_pos);
    freeBoard(game_board);

    return 0;
}