#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "piece.h"

void newBoard(piece board[8][8]);
bool isPosAccessible(piece board[8][8], int currentx, int currenty, int tox, int toy);
bool movePiece(piece board[8][8], int currentx, int currenty, int tox, int toy);
int isInChess(piece board[8][8], int kingposx, int kingposy);
bool isMate(piece board[8][8], int kingposx, int kingposy);
void printBoard(piece board[8][8]);