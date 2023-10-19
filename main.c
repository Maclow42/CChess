#include <ncurses.h>
#include <locale.h>

#include "./src/UI/game/game.h"
#include "./src/UI/menu/menu.h"

int main(){
    setlocale(LC_ALL, ""); //Config environment for UTF-8

    //Init ncurses with some options
    initscr(); // Init ncurses
    curs_set(0); //Disable cursor
    start_color(); // Ability to use colors
    noecho(); // Disable caractere echo
    mousemask(BUTTON1_CLICKED, NULL); // Enable mouse click detection

    //Init ncurses windows
    WINDOW *boite = newwin(0, 0, 0, 0);
    keypad(boite, TRUE); // Enable keypad special keys detection

    // Vérifie la prise en charge des couleurs
    if (!has_colors()) {
        endwin();
        printf("Your terminal does not support colors display.\n");
        return 1;
    }

    while(1){
        int nb_player = display_menu(boite);
        if(nb_player == -1)
            break;
        game_UI(boite, nb_player, WHITE);
    }

    endwin(); // Quit ncurses



    return 0;
}