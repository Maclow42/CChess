#include "game.h"

#include <ncurses.h>
#include <wchar.h>
#include <stdbool.h>
#include "../../gameboard/gameboard.h"
#include "../../gameboard/move_piece.h"
#include "../../gameboard/game_status.h"
#include "../../minmax/minmax.h"

#define WINDOW_HEIGHT 19
#define WINDOW_WIDTH 37

#define clearALL() {clear(); wrefresh(window); refresh();}
#define refreshALL() {wrefresh(window); refresh();}

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

void refresh_display(WINDOW* window, game_board* game_board, coords* selected_piece, color_t player_color_view){
    clearALL();

    // Allocate memory to stock the result[][] of printBoard
    char** result = (char**)malloc(20 * sizeof(char*));
    for (int i = 0; i < 20; ++i) {
        result[i] = (char*)malloc(102 * sizeof(char));
    }

    // Get the new string of board to display
    printBoard(game_board, player_color_view, result);
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
        if(player_color_view == WHITE)
            mvwaddstr(window, (8-selected_piece->posy)*2, selected_piece->posx*4+3, piece);
        else
            mvwaddstr(window, selected_piece->posy*2+2, selected_piece->posx*4+3, piece);
        wattroff(window, COLOR_PAIR(2));
    }

    refreshALL();

    // free the memory allocated to result
    for (int i = 0; i < 20; ++i) {
        free(result[i]);
    }
    free(result);
}

int game_UI(WINDOW* window, int nb_player, color_t player_color_view){
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
    
    // window's position to center it
    int start_y = (LINES - WINDOW_HEIGHT) / 2;
    int start_x = (COLS - WINDOW_WIDTH) / 2;
    mvwin(window, start_y, start_x);

    bool display = true;


    // Main loop
    int ch = -1; // char read by wgetch
    MEVENT event; // mouse event
    do{
        // dWindow display gestion
        // If window is resized
        if(ch == KEY_RESIZE){
            if(LINES < WINDOW_HEIGHT + 6 || COLS < WINDOW_WIDTH + 10)
                display = false;
            else{
                if(!display){
                    display = true;
                    wresize(window, WINDOW_HEIGHT, WINDOW_WIDTH);
                }

                start_y = (LINES - WINDOW_HEIGHT) / 2;
                start_x = (COLS - WINDOW_WIDTH) / 2;
                mvwin(window, start_y, start_x);
                refresh_display(window, game_board, NULL, player_color_view);
            }
        }
        
        // If window is too small to display the board
        if(!display){
            clearALL();
            char error_msg1[] = "The window is too small";
            char error_msg2[] = "to display the board";
            mvprintw(LINES/2, (COLS-strlen(error_msg1))/2, "%s", error_msg1);
            mvprintw(LINES/2+1, (COLS-strlen(error_msg2))/2, "%s", error_msg2);
            refreshALL();
            continue;
        }

        //if 1 player mode and it's the IA turn, play it
        if (nb_player == 1 && game_board->to_play != player_color_view){
            refresh_display(window, game_board, NULL, player_color_view);
            mvprintw(1, 1, "IA is playing...     ");
            refresh();
            game_board_play_AI(game_board);
        }

        else{
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
                        break;
                    default:
                        break;
                }
                // Reset start_pos and end_pos to NULL
                reset_to_NULL_Coords(&start_pos);
                reset_to_NULL_Coords(&end_pos);
            }

            // If mouse click event => get relative coordinates with wmouse_trafo
            if (ch == KEY_MOUSE) {
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
                if(player_color_view == WHITE)
                    posy = 7 - posy;
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
        }


        refresh_display(window, game_board, start_pos, player_color_view);

    }while(end_pos || (nb_player == 1 && game_board->to_play != player_color_view) || (ch = wgetch(window)) != 'q'); // Use 'q' to quit

    free(start_pos);
    free(end_pos);
    freeBoard(game_board);

    return 0;
}