#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 10

/*以下是所有可能生成的结构
1   2     3      4        5      6      7
█ ██ ██     ██     █   █         █
█ ██   ██ ██     ███ ███ ███
█
█
*/

typedef struct Block{
        int icon;
        int position[2];
}Block;

int out = 0;
Block blocks[4];
double centre[2];
int score = 0;
int next = 0;
int HEIGHT;
float SLEEP = 0;
int board[21][LENGTH];

int if_in(const int array[][2], const int array_length, const int y, const int x){
    for(int i=0;i<array_length;i++){
        if (array[i][0] == y && array[i][1] == x){
            return 1;
        }
    }
    return 0;
}

void r_rotate();
void l_rotate();
int try_fall();
void draw(const int y,const int x,int type);

//call()在键盘事件发生时被调用
void call(){
    const char KEY = _getch();
    switch(KEY){
        case 'r':r_rotate();break;
        case 'l':l_rotate();break;
        case 'a':l_move();break;//按a左移所有可控方块
        case 'd':r_move();break;//按d右移所有可控方块
        case 'p':out=2;break;
        case 'o':out=3;break;
        case 's':{//按s直接下落到底部
            for(int i=0;i<HEIGHT;i++){
                if (try_fall() == 1){
                    break;
                }
            }
            break;
        }
        default:return;
    }
}

//修改cmd输出颜色，输入为方块的值
void set_color(const int type){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch(type){
        case 0:SetConsoleTextAttribute(hConsole,0x07);break;
        case 1:SetConsoleTextAttribute(hConsole,0x99);break;
        case 2:SetConsoleTextAttribute(hConsole,0xaa);break;
        case 3:SetConsoleTextAttribute(hConsole,0xbb);break;
        case 4:SetConsoleTextAttribute(hConsole,0xcc);break;
        case 5:SetConsoleTextAttribute(hConsole,0xdd);break;
        case 6:SetConsoleTextAttribute(hConsole,0xee);break;
        case 7:SetConsoleTextAttribute(hConsole,0xff);break;
    }
}

//初始化游戏面板
void init_board(){
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<HEIGHT;j++){
            board[i][j] = 0;
        }
    }
}

//左移
void l_move(){
    int prev_pos[4][2];
    for(int i=0;i<4;i++){
        prev_pos[i][0] = blocks[i].position[0];
        prev_pos[i][1] = blocks[i].position[1];
    }
    for(int i=0;i<4;i++){
        if(blocks[i].position[1] <= 0 || (blocks[i].position[0] >= 0 && (board[blocks[i].position[0]][blocks[i].position[1]-1] != 0) && if_in(prev_pos, 4, blocks[i].position[0], blocks[i].position[1]-1) == 0)){
            return;
        }
    }
    for(int i=0;i<4;i++){
        draw(blocks[i].position[0],blocks[i].position[1],0);
    }
    for(int i=0;i<4;i++){
        blocks[i].position[1]--;
        draw(blocks[i].position[0],blocks[i].position[1],blocks[i].icon);
    }
    centre[1]--;
}

//右移
void r_move(){
    int prev_pos[4][2];
    for(int i=0;i<4;i++){
        prev_pos[i][0] = blocks[i].position[0];
        prev_pos[i][1] = blocks[i].position[1];

    }
    for(int i=0;i<4;i++){
        if(blocks[i].position[1] >= LENGTH-1 || (blocks[i].position[0] >= 0 && board[blocks[i].position[0]][blocks[i].position[1]+1] != 0 && if_in(prev_pos, 4, blocks[i].position[0], blocks[i].position[1]+1) == 0)){
            return;
        }
    }
    for(int i=0;i<4;i++){
        draw(blocks[i].position[0],blocks[i].position[1],0);
    }
    for(int i=0;i<4;i++){
        blocks[i].position[1]++;
        draw(blocks[i].position[0],blocks[i].position[1],blocks[i].icon);
    }
    centre[1]++;
}

//逆时针旋转
void l_rotate(){
    int prev_pos[4][2];
    for(int i=0;i<4;i++){
        prev_pos[i][0] = blocks[i].position[0];
        prev_pos[i][1] = blocks[i].position[1];
    }
    int temp[8];
    for(int i=0;i<4;i++){
        const int Y = (int)(centre[0]-blocks[i].position[1]+centre[1]);
        const int X = (int)(centre[1]+blocks[i].position[0]-centre[0]);
        if(X < 0 || X > LENGTH-1 || Y > HEIGHT-1 || (board[Y][X] != 0 && if_in(prev_pos, 4, Y, X) == 0)){
            return;
        }
        temp[i*2] = Y;
        temp[i*2+1] = X;
    }
    for(int i=0;i<4;i++){
        draw(blocks[i].position[0],blocks[i].position[1],0);
        blocks[i].position[0] = temp[i*2];
        blocks[i].position[1] = temp[i*2+1];
    }
    for(int i=0;i<4;i++){
        draw(blocks[i].position[0],blocks[i].position[1],blocks[i].icon);
    }
}

//顺时针旋转
void r_rotate(){
    int prev_pos[4][2];
    for(int i=0;i<4;i++){
        prev_pos[i][0] = blocks[i].position[0];
        prev_pos[i][1] = blocks[i].position[1];
    }
    int temp[8];
    for(int i=0;i<4;i++){
        const int Y = (int)(centre[0]+blocks[i].position[1]-centre[1]);
        const int X = (int)(centre[1]-blocks[i].position[0]+centre[0]);
        if (X < 0 || X > LENGTH-1 || Y > HEIGHT-1 || (board[Y][X] != 0 && if_in(prev_pos, 4, Y, X) == 0)){
            return;
        }
        temp[i*2] = Y;
        temp[i*2+1] = X;
    }
    for(int i=0;i<4;i++){
        draw(blocks[i].position[0],blocks[i].position[1],0);
        blocks[i].position[0] = temp[i*2];
        blocks[i].position[1] = temp[i*2+1];
    }
    for(int i=0;i<4;i++){
        draw(blocks[i].position[0],blocks[i].position[1],blocks[i].icon);
    }
}

//将一个指定颜色的方块绘入面板的指定区域
void draw(const int y, const int x, int type){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (y < 0)return;
    board[y][x] = type;
    COORD pos = {x*2+2,y+1};
    SetConsoleCursorPosition(hConsole,pos);
    set_color(type);
    printf("  ");
}

//尝试使所有可控方块下落，能下落返回False，不能下落返回True
int try_fall(){
    int prev_pos[4][2];
    for(int i=0;i<4;i++){
        (*(prev_pos+i))[0] = blocks[i].position[0];
        (*(prev_pos+i))[1] = blocks[i].position[1];
    }
    for(int i=0;i<4;i++){
        if(blocks[i].position[0] >= HEIGHT - 1 || (blocks[i].position[0] >= -1 && board[blocks[i].position[0]+1][blocks[i].position[1]] != 0 && if_in(prev_pos, 4, blocks[i].position[0]+1, blocks[i].position[1]) == 0)){
            return 1;
        }
    }
    for(int i=0;i<4;i++){
        draw(blocks[i].position[0],blocks[i].position[1],0);
    }
    for(int i=0;i<4;i++){
        blocks[i].position[0]++;
        draw(blocks[i].position[0],blocks[i].position[1],blocks[i].icon);
    }
    centre[0]++;
    return 0;
}

//仅在try_fall()返回True时被调用，进行得分与层的消除
void try_score(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int add = 1;
    for(int i=0;i<HEIGHT;i++){
        int if_score = 1;
        for(int j=0;j<LENGTH;j++){
            if (board[i][j] == 0){
                if_score = 0;
                break;
            }
        }
        if (if_score){
            score += LENGTH * add++;
            for(int k=i;k>-1;k--){
                if (k != 0){
                    for (int l=0;l<LENGTH;l++){
                        draw(k,l,board[k-1][l]);
                    }
                }else{
                    for (int l=0;l<LENGTH;l++){
                        draw(k,l,0);
                    }
                }
            }
        }
    }
    COORD pos={13+LENGTH*2,10};
    SetConsoleCursorPosition(hConsole,pos);
    SetConsoleTextAttribute(hConsole,0x07);
    printf("%d",score);
}

//生成结构
void structure(int block_type){
    if (block_type == 1){//1型
        const int X = rand()%(LENGTH - 3);
        for(int i=0;i<4;i++){
            Block block;
            block.icon = 1;
            block.position[0] = -1;
            block.position[1] = X+i;
            *(blocks+i) = block;
        }
        centre[0] = -1.5;
        centre[1] = X+1.5;
    }else if (block_type == 2){//O型
        const int X = rand()%(LENGTH - 1);
        for (int i=0;i<2;i++){
            for (int j=0;j<2;j++){
            Block block;
            block.icon = 2;
            block.position[0] = -1-i;
            block.position[1] = X+j;
            *(blocks+i*2+j) = block;
            }
        }
        centre[0] = -1.5;
        centre[1] = X+0.5;
    }else if (block_type == 3){//Z型
        const int X = rand()%(LENGTH - 2);
        for (int i=0;i<2;i++){
            for (int j=0;j<2;j++){
                Block block;
                block.icon = 3;
                block.position[0] = -1-i;
                block.position[1] = X-i+j+1;
                *(blocks+i*2+j) = block;
            }
        }
        centre[0] = -2;
        centre[1] = X+1;
    }else if (block_type == 4){//S型
        const int X = rand()%(LENGTH - 2);
        for (int i=0;i<2;i++){
            for (int j=0;j<2;j++){
                Block block;
                block.icon = 4;
                block.position[0] = -1-i;
                block.position[1] = X+i+j;
                *(blocks+i*2+j) = block;
            }
        }
        centre[0] = -2;
        centre[1] = X+1;
    }else if (block_type == 5){//T型
        const int X = rand()%(LENGTH - 2);
        for (int i=0;i<3;i++){
            Block block;
            block.icon = 5;
            block.position[0] = -1;
            block.position[1] = X+i;
            *(blocks+i) = block;
        Block block1;
        block1.icon = 5;
        block1.position[0] = -2;
        block1.position[1] = X+1;
        *(blocks+3) = block1;
        }
        centre[0] = -1;
        centre[1] = X+1;
    }else if (block_type == 6){//J型
        const int X = rand()%(LENGTH - 2);
        for (int i=0;i<3;i++){
            Block block;
            block.icon = 6;
            block.position[0] = -1;
            block.position[1] = X+i;
            *(blocks+i) = block;
        Block block1;
        block1.icon = 6;
        block1.position[0] = -2;
        block1.position[1] = X;
        *(blocks+3) = block1;
        }
        centre[0] = -1;
        centre[1] = X+1;
    }else if (block_type == 0){//L型
        const int X = rand()%(LENGTH - 2);
        for (int i=0;i<3;i++){
            Block block;
            block.icon = 7;
            block.position[0] = -1;
            block.position[1] = X+i;
            *(blocks+i) = block;
        Block block1;
        block1.icon = 7;
        block1.position[0] = -2;
        block1.position[1] = X+2;
        *(blocks+3) = block1;
        }
        centre[0] = -1;
        centre[1] = X+1;
    }
}

//游戏面板输出
void print_board(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");
    printf("┌");
    for(int i=0;i<LENGTH;i++){
        printf("─");
    }
    printf("┐\n");
    for(int i=0;i<HEIGHT;i++){
        printf("│");
        for(int j=0;j<LENGTH;j++){
            printf("  ");
        }
        printf("│\n");
    }
    printf("└");
    for(int i=0;i<LENGTH;i++){
        printf("─");
    }
    printf("┘\n");
}

//判断游戏是否须结束
int if_over(){
    for(int i=0;i<4;i++){
        if (blocks[i].position[0] < 0){
            return 1;
        }
    }
    return 0;
}

//打印UI操作提示
void print_ui(int mode){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {2*LENGTH+6,3};
    SetConsoleCursorPosition(hConsole,pos);
    switch(mode){
        case 1:printf("mode: easy\n");break;
        case 2:printf("mode: normal\n");break;
        case 3:printf("mode: hard\n");break;
        case 4:printf("mode: lunatic\n");break;
    }

    pos.Y = 5;
    SetConsoleCursorPosition(hConsole,pos);
    printf("next block:");

    pos.Y = 10;
    SetConsoleCursorPosition(hConsole,pos);
    printf("score: %d\n",score);

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

//打印下一个方块
void print_next(type){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    //清除前一次的图案，防止重叠
    set_color(0);
    COORD pos = {2*LENGTH+8,7};
    SetConsoleCursorPosition(hConsole,pos);
    printf("        ");
    pos.Y++;
    SetConsoleCursorPosition(hConsole,pos);
    printf("        ");

    //绘制新图案
    set_color(type);
    if(type == 0)set_color(7);
    switch(type){
        case 1:{
            COORD pos = {2*LENGTH+8,7};
            SetConsoleCursorPosition(hConsole,pos);
            printf("        ");
            break;
        }
        case 2:{
            COORD pos = {2*LENGTH+8,7};
            SetConsoleCursorPosition(hConsole,pos);
            printf("    ");
            pos.Y++;
            SetConsoleCursorPosition(hConsole,pos);
            printf("    ");
            break;
        }
        case 3:{
            COORD pos = {2*LENGTH+8,7};
            SetConsoleCursorPosition(hConsole,pos);
            printf("    ");
            pos.Y++;
            pos.X += 2;
            SetConsoleCursorPosition(hConsole,pos);
            printf("    ");
            break;
        }
        case 4:{
            COORD pos = {2*LENGTH+10,7};
            SetConsoleCursorPosition(hConsole,pos);
            printf("    ");
            pos.Y++;
            pos.X -= 2;
            SetConsoleCursorPosition(hConsole,pos);
            printf("    ");
            break;
        }
        case 5:{
            COORD pos = {2*LENGTH+10,7};
            SetConsoleCursorPosition(hConsole,pos);
            printf("  ");
            pos.Y++;
            pos.X -= 2;
            SetConsoleCursorPosition(hConsole,pos);
            printf("      ");
            break;
        }
        case 6:{
            COORD pos = {2*LENGTH+10,7};
            SetConsoleCursorPosition(hConsole,pos);
            printf("  ");
            pos.Y++;
            SetConsoleCursorPosition(hConsole,pos);
            printf("      ");
            break;
        }
        case 0:{
            COORD pos = {2*LENGTH+12,7};
            SetConsoleCursorPosition(hConsole,pos);
            printf("  ");
            pos.Y++;
            pos.X -= 4;
            SetConsoleCursorPosition(hConsole,pos);
            printf("      ");
            break;
        }
    }
}

//每隔SLEEP秒被调用一次
void update(){
    if(try_fall()){
        if(if_over()){
            out = 1;
        }
        try_score();
        structure(next);
        next = rand()%7;
        print_next(next);
    }
}

//将光标移动到控制台(x,y)坐标处
void go_to_x_y(int x,int y){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos={x,y};
    SetConsoleCursorPosition(hConsole,pos);
}

//打印选择难度界面
void print_choice(int choice){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (choice){
        case 1:{
            go_to_x_y(0,3);
            SetConsoleTextAttribute(hConsole,0xf0);
            printf("easy");
            SetConsoleTextAttribute(hConsole,0x0f);
            printf(" normal hard lunatic");
            SetConsoleTextAttribute(hConsole,0x07);
            break;
        }
        case 2:{
            go_to_x_y(0,3);
            SetConsoleTextAttribute(hConsole,0x0f);
            printf("easy ");
            SetConsoleTextAttribute(hConsole,0xf0);
            printf("normal");
            SetConsoleTextAttribute(hConsole,0x0f);
            printf(" hard lunatic");
            SetConsoleTextAttribute(hConsole,0x07);
            break;
        }
        case 3:{
            go_to_x_y(0,3);
            SetConsoleTextAttribute(hConsole,0x0f);
            printf("easy normal ");
            SetConsoleTextAttribute(hConsole,0xf0);
            printf("hard");
            SetConsoleTextAttribute(hConsole,0x0f);
            printf(" lunatic");
            SetConsoleTextAttribute(hConsole,0x07);
            break;
        }
        case 4:{
            go_to_x_y(0,3);
            SetConsoleTextAttribute(hConsole,0x0f);
            printf("easy normal hard ");
            SetConsoleTextAttribute(hConsole,0xf0);
            printf("lunatic");
            SetConsoleTextAttribute(hConsole,0x07);
            break;
        }
    }
}

//选择游戏难度
int choose_mode(){
    system("cls");
    printf("choose gamemode:\na for left, d for right, s for choice.\n");
    int choice = 1;
    print_choice(choice);
    int if_choosing = 1;
    while(if_choosing){
        if(_kbhit()){
            const char KEY = _getch();
            switch(KEY){
                case 'a':{
                    if(choice > 1)choice--;
                    else choice = 4;
                    print_choice(choice);
                    break;
                }
                case 'd':{
                    if(choice < 4)choice++;
                    else choice = 1;
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

    switch(choice){
    case 1:
        SLEEP = 0.5;HEIGHT = 21;break;
    case 2:
        SLEEP = 0.4;HEIGHT = 20;break;
    case 3:
        SLEEP = 0.3;HEIGHT = 19;break;
    case 4:
        SLEEP = 0.2;HEIGHT = 18;break;
    }
    return choice;
}

//主函数
int main(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

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
    _getch();

    int mode = choose_mode();

    srand(time(0));

    //初始化游戏面板并打印
    init_board();
    print_board();
    print_ui(mode);

    structure(rand()%7);
    next = rand()%7;
    print_next(next);

    //游戏进行中的循环
    while(out != 1){
        if(out == 2){
            COORD pos={0,3+HEIGHT};
            SetConsoleCursorPosition(hConsole,pos);
            SetConsoleTextAttribute(hConsole,0x07);
            printf("Game paused. Press any key to continue.\n");
            _getch();
            SetConsoleCursorPosition(hConsole,pos);
            printf("                                       ");
            out = 0;
        }else if(out == 3){
            return 0;
        }
        float prev_time = (float)GetTickCount64()/1000;
        while((float)GetTickCount64()/1000-prev_time < SLEEP){
            /*if((double)GetTickCount64()/1000-prev_time< SLEEP/3){
                continue;
            }*/
            if(_kbhit()){
            call();
            }
        }
        update();
    }
    COORD pos={0,3+HEIGHT};
    SetConsoleCursorPosition(hConsole,pos);
    SetConsoleTextAttribute(hConsole,0x07);
    printf("Game over! Press any key for exit!\n");
    _getch();
    return 0;
}
