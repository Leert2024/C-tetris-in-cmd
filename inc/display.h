# ifndef DISPLAY_H
# define DISPLAY_H

# include <windows.h>
# include <stdio.h>
# include "main.h"

void Console_HideCursor(void);
void Console_GoTo(int x,int y);
void Console_SetColor(int color);

void Display_SetColor(int type);
void Display_PrintHello(void);
void Display_PrintBoard(Game_t game);
void Display_PrintNext(Game_t game);
void Display_PrintUI(Game_t game);

# endif
