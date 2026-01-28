# include "display.h"

const int colors[8] = {0x07, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

extern const Block_t pattern_relative[8][4];    // 图案相对坐标，定义在main.c

/** ==================== 方法 ==================== */

/**
 * @brief 隐藏光标
 */
void Console_HideCursor(void){
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
}

/**
 * @brief 将光标移动到控制台(x,y)坐标处
 * @param x 横坐标
 * @param y 纵坐标
 */
void Console_GoTo(int x,int y){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos={x,y};
    SetConsoleCursorPosition(hConsole,pos);
}

/**
 * @brief 设置控制台文本颜色
 * @param color 颜色值
 */
void Console_SetColor(int color){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,color);
}

/** ==================== 具体功能实现 ==================== */

/**
 * @brief 根据方块类型设置控制台文本颜色
 * @param type 方块类型
 */
void Display_SetColor(int type){
    Console_SetColor(colors[type]);
}

/**
 * @brief   打印欢迎界面
 */
void Display_PrintHello(void){
    Console_GoTo(0,0);
    Console_SetColor(0x07);

    printf("Tetris by Github user Leert2024\n");
    printf("俄罗斯方块，开发者Github：Leert2024\n\n");

    printf("Gameplay:\n");
    printf("游戏规则：\n");

    printf("Press s for instant fall.\n");
    printf("按s键使所有方块立即下落；\n");

    printf("d for right.a for left.\n");
    printf("按d右移；按a左移；\n");

    printf("r for clockwise rotation.\n");
    printf("按r顺时针旋转；\n");

    printf("l for anticlockwise rotation.\n");
    printf("按l逆时针旋转；\n");

    printf("p for pause.o for exit.\n");
    printf("按p暂停；按o结束游戏。\n\n");

    printf("(Attention: Please set keyboard to English)\n");
    printf("（注意：请将输入法切换为英文进行游戏）\n\n");

    printf("Press any key to start...\n");
    printf("按任意键开始游戏……\n");
}

/**
 * @brief   打印游戏面板
 * @param   game 游戏句柄
 */
void Display_PrintBoard(Game_t game){
    system("cls");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,0x88);

    Console_GoTo(0,0);
    printf("  ");
    for(int i=0;i<game.board.width;i++){
        printf("  ");
    }
    printf("  ");
    for(int i=0;i<game.board.height;i++){
        Console_GoTo(0,i+1);
        printf("  ");
        for(int j=0;j<game.board.width;j++){
            Console_SetColor(colors[game.board.addr[i][j]]);
            printf("  ");
        }
        SetConsoleTextAttribute(hConsole,0x88);
        printf("  ");
    }

    Console_GoTo(0,game.board.height+1);
    printf("  ");
    for(int i=0;i<game.board.width;i++){
        printf("  ");
    }
    printf("  ");
}

/**
 * @brief   打印下一个图案
 * @param   game 游戏句柄
 */
void Display_PrintNext(Game_t game){
    //清除前一次的图案，防止重叠
    Console_SetColor(colors[0]);
    Console_GoTo(2*WIDTH+8, 7);
    printf("        ");
    Console_GoTo(2*WIDTH+8, 8);
    printf("        ");

    //绘制新图案
    int type = game.next_type;
    if(type == 0)return;
    Console_SetColor(colors[type]);
    for(int i=0;i<4;i++){
        Console_GoTo(2*WIDTH+8+2*pattern_relative[type][i].x, 7+pattern_relative[type][i].y);
        printf("  ");
    }
}

/**
 * @brief   打印UI操作提示
 * @param   game 游戏句柄
 */
void Display_PrintUI(Game_t game){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,0x07);
    COORD pos = {2*WIDTH+6,3};
    SetConsoleCursorPosition(hConsole,pos);
    switch(game.difficulty){
        case 0:printf("mode: easy\n");break;
        case 1:printf("mode: normal\n");break;
        case 2:printf("mode: hard\n");break;
        case 3:printf("mode: lunatic\n");break;
    }

    pos.Y = 5;
    SetConsoleCursorPosition(hConsole,pos);
    printf("next block:");

    pos.Y = 10;
    SetConsoleCursorPosition(hConsole,pos);
    printf("score: %d\n",game.score);

    pos.Y ++;
    SetConsoleCursorPosition(hConsole,pos);
    printf("Press s for instant fall.\n");

    pos.Y ++;
    SetConsoleCursorPosition(hConsole,pos);
    printf("d for right.\ta for left.\n");

    pos.Y ++;
    SetConsoleCursorPosition(hConsole,pos);
    printf("r for clockwise rotation.\n");

    pos.Y ++;
    SetConsoleCursorPosition(hConsole,pos);
    printf("l for anticlockwise rotation.\n");

    pos.Y ++;
    SetConsoleCursorPosition(hConsole,pos);
    printf("p for pause.\to for exit\n");
}
