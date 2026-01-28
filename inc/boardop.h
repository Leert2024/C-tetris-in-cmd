# ifndef BOARDOP_H
# define BOARDOP_H

# include "main.h"
# include "display.h"

int Board_MoveLeft(Board_t* board, Pattern_t* pattern);
int Board_MoveRight(Board_t* board, Pattern_t* pattern);
int Board_MoveDown(Board_t* board, Pattern_t* pattern);
int Board_RotateLeft(Board_t* board, Pattern_t* pattern);
int Board_RotateRight(Board_t* board, Pattern_t* pattern);
int Board_Eliminate(Board_t *board);

# endif
