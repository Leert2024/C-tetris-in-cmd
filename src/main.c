# include <stdio.h>
# include <time.h>
# include <windows.h>
# include <stdlib.h>
# include <string.h>
# include <conio.h>
# include <stdint.h>
# include "main.h"
# include "display.h"
# include "fileio.h"
# include "boardop.h"

/*
1    2     3       4       5      6      7
[]  [][] []()      ()[]  []         []   []
()  [][]   [][]  [][]    []()[] []()[] []()[]
[]
[]
*/

const int heights[4] = {20, 18, 16, 14};
const int sleep_time[4] = {1000, 800, 600, 400};
const Block_t pattern_relative[8][4] = {
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
    {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
    {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
    {{1, 0}, {2, 0}, {0, 1}, {1, 1}},
    {{0, 0}, {0, 1}, {1, 1}, {2, 1}},
    {{2, 0}, {0, 1}, {1, 1}, {2, 1}},
    {{1, 0}, {0, 1}, {1, 1}, {2, 1}},
};
const Block_t pattern_centre[8] = {
    {0, 0}, 
    {1, 0},     // 长条
    {0, 0},     // 方块
    {1, 0},     // Z形
    {1, 0},     // S形
    {1, 1},     // L形
    {1, 1},     // J形
    {1, 1},     // T形
};
const int pattern_width[8] = {0, 4, 2, 3, 3, 3, 3, 3};
const int pattern_height[8] = {0, 1, 2, 2, 2, 2, 2, 2};

/* 游戏句柄 */
Game_t game;

int OutFlag = 0;    // 游戏退出标志，1表示失败退出，2表示玩家主动退出

/* 键盘事件响应 */
void keydown(char key){
    switch(key){
        case 'r':Board_RotateRight(&game.board, &game.pattern); break;
        case 'l':Board_RotateLeft(&game.board, &game.pattern); break;
        case 'a':Board_MoveLeft(&game.board, &game.pattern); break;
        case 'd':Board_MoveRight(&game.board, &game.pattern); break;
        case 's':
            for(int i = 0; i < game.board.height; i++){
                if (Board_MoveDown(&game.board, &game.pattern) == 0){
                    break;
                }
            }
            break;
        case 'p':
            Console_GoTo(0, 3 + game.board.height);
            Console_SetColor(0x07);
            printf("Game paused. Press any key to continue.                   \n");
            _getch();
            Console_GoTo(0,3 + game.board.height);
            printf("                                                          ");
            break;
        case 'o': OutFlag = 2; break;
        default: break;
    }
}

/* 更新游戏得分 */
void Game_UpdateScore(int dscore){
    game.score += dscore;
    Console_GoTo(13 + game.board.width*2, 10);
    Console_SetColor(0x07);
    printf("%d",game.score);
}

/* 生成新的可控图案 */
void Game_GeneratePattern(void){
    int x = rand()%(WIDTH-pattern_width[game.pattern.type]);
    int y = 0 - pattern_height[game.pattern.type];
    for(int i = 0; i < 4; i++){
        game.pattern.blocks[i].x = x + pattern_relative[game.pattern.type][i].x;
        game.pattern.blocks[i].y = y + pattern_relative[game.pattern.type][i].y;
    }
    game.pattern.centre.x = x + pattern_centre[game.pattern.type].x;
    game.pattern.centre.y = y + pattern_centre[game.pattern.type].y;
}

/* 判断游戏是否须结束 */
int Game_IsOver(void){
    for(int i=0;i<4;i++){
        if (game.pattern.blocks[i].y < 0){
            return 1;
        }
    }
    return 0;
}

/* 每隔SLEEP秒被调用一次 */
void Game_Update(){
    if(!Board_MoveDown(&game.board, &game.pattern)){
        int eliminate_num = Board_Eliminate(&game.board);
        Game_UpdateScore((eliminate_num + 1)*eliminate_num/2*game.board.width);
        if(Game_IsOver()){
            OutFlag = 1;
            return;
        }
        game.pattern.type = game.next_type;
        game.next_type = rand()%7 + 1;
        Game_GeneratePattern();
        Display_PrintNext(game);
    }
    FILEIO_WriteInfo(game);//存档
}

/* 打印选择难度选项 */
void print_choice(int choice){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (choice){
        case 0:{
            Console_GoTo(0,3);
            Console_SetColor(0xf0);
            printf("easy");
            Console_SetColor(0x0f);
            printf(" normal hard lunatic");
            Console_SetColor(0x07);
            break;
        }
        case 1:{
            Console_GoTo(0,3);
            Console_SetColor(0x0f);
            printf("easy ");
            Console_SetColor(0xf0);
            printf("normal");
            Console_SetColor(0x0f);
            printf(" hard lunatic");
            Console_SetColor(0x07);
            break;
        }
        case 2:{
            Console_GoTo(0,3);
            Console_SetColor(0x0f);
            printf("easy normal ");
            Console_SetColor(0xf0);
            printf("hard");
            Console_SetColor(0x0f);
            printf(" lunatic");
            Console_SetColor(0x07);
            break;
        }
        case 3:{
            Console_GoTo(0,3);
            Console_SetColor(0x0f);
            printf("easy normal hard ");
            Console_SetColor(0xf0);
            printf("lunatic");
            Console_SetColor(0x07);
            break;
        }
    }
}

/* 打印打开旧有存档/开启新游戏选项 */
void print_old_or_new(int choice){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch(choice){
        case 0:{
            Console_GoTo(0,5);
            SetConsoleTextAttribute(hConsole,0x0f);
            printf("new game ");
            SetConsoleTextAttribute(hConsole,0xf0);
            printf("prev game");
            SetConsoleTextAttribute(hConsole,0x07);
            printf("\n\n");
            break;
        }
        case 1:{
            Console_GoTo(0,5);
            SetConsoleTextAttribute(hConsole,0xf0);
            printf("new game");
            SetConsoleTextAttribute(hConsole,0x0f);
            printf(" prev game");
            SetConsoleTextAttribute(hConsole,0x07);
            printf("\n\n");
            break;
        }
    }
}

/* 选择游戏难度 */
int Stage_ChooseDifficulty(void){
    system("cls");
    printf("choose gamemode:\na for left, d for right, s for choice.\n");
    int choice = 0;
    print_choice(choice);
    int if_choosing = 1;
    while(if_choosing){
        if(_kbhit()){
            const char KEY = _getch();
            switch(KEY){
                case 'a':{
                    if(choice > 0)choice--;
                    else choice = 3;
                    print_choice(choice);
                    break;
                }
                case 'd':{
                    if(choice < 3)choice++;
                    else choice = 0;
                    print_choice(choice);
                    break;
                }
                case 's':{
                    if_choosing = 0;
                    break;
                }
                default:break;
            }
        }
    }
    return choice;
}

/* 尝试打开已有存档，成功返回1，失败返回0 */
int Game_LoadPrev(void){
    int result = FILEIO_ReadInfo(&game);
    if(!result){
        printf("Fail to load saved game. The file may have been damaged or deleted.\n");
        printf("打开存档失败，存档可能已被删除或损坏。\n\n");
        printf("Trying to create new game...\n");
        printf("正在尝试打开新游戏……\n\n");
        Sleep(1000);
    }
}

/* 初始化游戏 */
void Game_Init(int difficulty){
    game.difficulty = difficulty;
    game.board.width = WIDTH;
    game.board.height = heights[game.difficulty];
    memset(game.board.addr, 0, sizeof(game.board.addr));
    game.score = 0;
    game.pattern.type = rand()%7 + 1;
    Game_GeneratePattern();
    game.next_type = rand()%7 + 1;
}

/* 尝试创建新存档，成功返回1，失败返回0 */
int Game_CreateNew(int difficulty){
    Game_Init(difficulty);
    if(!FILEIO_WriteInfo(game)){
        printf("Fail to create new game! Please try again.\n");
        printf("无法打开新游戏！请重试。\n\n");
        Sleep(500);
        return 0;
    }
    return 1;
}

/* 欢迎界面 */
void Stage_Hello(void){
    Console_HideCursor();
    Display_PrintHello();
    _getch();
}

/* 选择打开存档/开启新存档 */
int Stage_NewOrOld(void){
    int out = 0;
    while(!out){
        system("cls");
        printf("Open previous game or create new game?\n");
        printf("打开旧有存档或开始新游戏?\n\n");
        printf("a for left, d for right, s for choice.\n");
        char key = 'w';
        int choice = 1;
        print_old_or_new(choice);
        while(key != 's'){
            key = _getch();
            switch(key){
                case 'a':{
                    if(choice > 0)choice--;
                    else choice = 1;
                    print_old_or_new(choice);
                    break;
                }
                case 'd':{
                    if(choice < 1)choice++;
                    else choice = 0;
                    print_old_or_new(choice);
                    break;
                }
                default:break;
            }
        }
        return choice;
    }
}

/* 进入游戏 */
void Stage_Game(void){
    volatile static uint64_t prev_time = 0;
    Display_PrintBoard(game);
    Display_PrintUI(game);
    Display_PrintNext(game);
    //游戏进行中的循环
    while(OutFlag != 1){
        prev_time = GetTickCount64();
        while((GetTickCount64() - prev_time) < sleep_time[game.difficulty]){
            uint64_t cur_time = GetTickCount64();
            if(_kbhit()){
                char key = _getch();
                keydown(key);
            }

        }
        if(OutFlag == 2){
            return;
        }
        Game_Update();
    }
    Console_GoTo(0,3+game.board.height);
    Console_SetColor(0x07);
    printf("Game over! Press any key for exit!\n                                ");
    _getch();
}

int main(){
    srand(time(0));
    Stage_Hello();
    while(1){
        int isNew = Stage_NewOrOld();
        if(isNew){
            int difficulty = Stage_ChooseDifficulty();
            if(Game_CreateNew(difficulty))break;
        }else{
            if(Game_LoadPrev())break;
            else{
                int difficulty = Stage_ChooseDifficulty();
                if(Game_CreateNew(difficulty))break;
            }
        }
    }
    Stage_Game();
    return 0;
}
