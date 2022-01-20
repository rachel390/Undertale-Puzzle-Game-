#ifndef GAME_H
#define GAME_H

#include "gba.h"

                    /* TODO: */

            // Create any necessary structs //

/*
* For example, for a Snake game, one could be:
*
* typedef struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* } Snake;
*
* Example of a struct to hold state machine data:
*
* typedef struct state {
*   int currentState;
*   int nextState;
* } State
*
*/

typedef struct frisk {
        int row;
        int col;
        int height;
        int length;
        int lastr;
        int lastc;
        int dir;
        unsigned short color;
} Frisk;

typedef struct square {
        int row;
        int col;
        unsigned short color;
        int number;
} Square;

typedef struct board {
        int numSquares;
        Square squares[121];
    
} Board;


void move(Frisk frisk);
void setPlay(Frisk frisk);
void setBoard(Board board);

#endif
