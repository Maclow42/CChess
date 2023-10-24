#include "color_choice.h"
#include <string.h>
#include <stdlib.h>

int display_color_choice(WINDOW *window) {
    clear();
    wclear(window);
    wrefresh(window);
    refresh();

    int height = 10;
    int width = 30;
    
    char *title = " CHESS MENU " ;
    int length = strlen(title);

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);

    int highlight = 1;
    int choice = 0;
    int ch;
    do{
        // Clear old display
        clear();

        // Update window size and position to always be centered
        int start_y = (LINES - height) / 2;
        int start_x = (COLS - width) / 2;
        wresize(window, height, width);
        mvwin(window, start_y, start_x);
        box(window, 0, 0);

        // Display title and 'q' to qui tinstruction
        mvwprintw(window, 0, width/2 - length/2, "%s", title);
        mvprintw(LINES - 2, 1, "Press 'q' to quit");

        //display choices
        char *choices[] = {"Play as White", "Play as Black"};
        int x = width/4;
        int y = height/2-1;
        for (int i = 0; i < 2; ++i) {
            if (highlight == i + 1) {
                wattron(window, COLOR_PAIR(2));
                mvwprintw(window, y, x, "> %s", choices[i]);
                wattroff(window, COLOR_PAIR(2));
            } else
                mvwprintw(window, y, x, "  %s", choices[i]);
            ++y;
        }

        refresh();

        // Choice selection with arrow keys
        ch = wgetch(window);

        switch (ch) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = 2;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == 2)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10: // Key Enter
                choice = highlight;
                break;
            default:
                break;
        }

        if (choice != 0) // Sort de la boucle si une option est sélectionnée
            break;
    }while(ch != 'q'); // Use 'q' to quit


    return ch == 'q' ? -1 : choice;
}