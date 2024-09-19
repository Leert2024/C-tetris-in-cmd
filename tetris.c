#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 10

/*���������п������ɵĽṹ
1   2     3      4        5      6      7
�� ���� ����     ����     ��   ��         ��
�� ����   ���� ����     ������ ������ ������
��
��
*/

typedef struct Block{
        int icon;
        int position[2];
}Block;

int out = 0;
Block blocks[4];
float centre[2];
int mode = 5;
int score = 0;
int next = 0;
int HEIGHT;
float SLEEP = 0;//������ʱ��
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

//call()�ڼ����¼�����ʱ������
void call(){
    const char KEY = _getch();
    switch(KEY){
        case 'r':r_rotate();break;
        case 'l':l_rotate();break;
        case 'a':l_move();break;//��a�������пɿط���
        case 'd':r_move();break;//��d�������пɿط���
        case 'p':out=2;break;
        case 'o':out=3;break;
        case 's':{//��sֱ�����䵽�ײ�
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

//�޸�cmd�����ɫ������Ϊ�����ֵ
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

//��ʼ����Ϸ���
void init_board(){
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<HEIGHT;j++){
            board[i][j] = 0;
        }
    }
}

//����
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

//����
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

//��ʱ����ת
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

//˳ʱ����ת
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

//��һ��ָ����ɫ�ķ����������ָ������
void draw(const int y, const int x, int type){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (y < 0)return;
    board[y][x] = type;
    COORD pos = {x*2+2,y+1};
    SetConsoleCursorPosition(hConsole,pos);
    set_color(type);
    printf("  ");
}

//����ʹ���пɿط������䣬�����䷵��False���������䷵��True
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

//���浱ǰ��Ϸ����
void save(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE* fp;
    if((fp = fopen("prev.dat","wb")) == NULL){
        go_to_x_y(0,3+HEIGHT);
        SetConsoleTextAttribute(hConsole,0x07);
        printf("Failed to save this step. New attempts will be made later.\n");
    }else{
        go_to_x_y(0,3+HEIGHT);
        SetConsoleTextAttribute(hConsole,0x07);
        printf("                                                          \n");
        /*
        ���ݱ���˳��
        1.��Ϸ���
        2.�ɿط�����Ϣ
        3.��ת����λ��
        4.��ǰ����
        5.��һ�ַ��������
        6.�Ѷ�
        */
        fwrite(board,sizeof(board),1,fp);
        fwrite(blocks,sizeof(Block),4,fp);
        fwrite(centre,sizeof(float),2,fp);
        fwrite(&score,sizeof(int),1,fp);
        fwrite(&next,sizeof(int),1,fp);
        fwrite(&mode,sizeof(int),1,fp);
        fclose(fp);
    }
}

//����try_fall()����Trueʱ�����ã����е÷���������
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

//���ɽṹ
void structure(int block_type){
    if (block_type == 1){//1��
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
    }else if (block_type == 2){//O��
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
    }else if (block_type == 3){//Z��
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
    }else if (block_type == 4){//S��
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
    }else if (block_type == 5){//T��
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
    }else if (block_type == 6){//J��
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
    }else if (block_type == 0){//L��
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

//��Ϸ������
void print_board(){
    system("cls");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,0x88);

    go_to_x_y(0,0);
    printf("  ");
    for(int i=0;i<LENGTH;i++){
        printf("  ");
    }
    printf("  ");
    for(int i=0;i<HEIGHT;i++){
        go_to_x_y(0,i+1);
        printf("  ");
        for(int j=0;j<LENGTH;j++){
            set_color(board[i][j]);
            printf("  ");
        }
        SetConsoleTextAttribute(hConsole,0x88);
        printf("  ");
    }

    go_to_x_y(0,HEIGHT+1);
    printf("  ");
    for(int i=0;i<LENGTH;i++){
        printf("  ");
    }
    printf("  ");
}

//�ж���Ϸ�Ƿ������
int if_over(){
    for(int i=0;i<4;i++){
        if (blocks[i].position[0] < 0){
            return 1;
        }
    }
    return 0;
}

//��ӡUI������ʾ
void print_ui(int mode){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,0x07);
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

//��ӡ��һ������
void print_next(type){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    //���ǰһ�ε�ͼ������ֹ�ص�
    set_color(0);
    COORD pos = {2*LENGTH+8,7};
    SetConsoleCursorPosition(hConsole,pos);
    printf("        ");
    pos.Y++;
    SetConsoleCursorPosition(hConsole,pos);
    printf("        ");

    //������ͼ��
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

//ÿ��SLEEP�뱻����һ��
void update(){
    if(try_fall()){
        try_score();
        if(if_over()){
            out = 1;
    return;
        }
        structure(next);
        next = rand()%7;
        print_next(next);
    }
    save();//�浵
}

//������ƶ�������̨(x,y)���괦
void go_to_x_y(int x,int y){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos={x,y};
    SetConsoleCursorPosition(hConsole,pos);
}

//��ӡѡ���ѶȽ���
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

//ѡ����Ϸ�Ѷ�
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
    return choice;
}

//���Դ����д浵���ɹ�����1��ʧ�ܷ���0
int load_prev(){
    FILE* fp;
    if((fp = fopen("prev.dat","rb"))==NULL){
        printf("Fail to load saved game. The file may have been damaged or deleted.\n");
        printf("�򿪴浵ʧ�ܣ��浵�����ѱ�ɾ�����𻵡�\n\n");
        Sleep(500);

        printf("Trying to create new game...\n");
        printf("���ڳ��Դ�����Ϸ����\n\n");
        Sleep(500);

        if(!create_new()){
            return 0;
        }else{
            return 1;
        }
    }else{
        /*
        ���ݶ�ȡ˳��
        1.��Ϸ���
        2.�ɿط�����Ϣ
        3.��ת����λ��
        4.��ǰ����
        5.��һ�ַ��������
        6.�Ѷ�
        */
        fread(board,sizeof(board),1,fp);
        fread(blocks,sizeof(Block),4,fp);
        fread(centre,sizeof(float),2,fp);
        fread(&score,sizeof(int),1,fp);
        fread(&next,sizeof(int),1,fp);
        fread(&mode,sizeof(int),1,fp);
        fclose(fp);
        return 1;
    }
}

//���Դ����´浵���ɹ�����1��ʧ�ܷ���0
int create_new(){
    FILE* fp;
    if((fp = fopen("prev.dat","wb"))==NULL){
        printf("Fail to create new game! Please try again.\n");
        printf("�޷�������Ϸ�������ԡ�\n\n");
        Sleep(1000);
        return 0;
    }else{
        init_board();
        structure(rand()%7);
        next = rand()%7;
        //TODO
        fwrite(board,sizeof(board),1,fp);
        fwrite(blocks,sizeof(Block),4,fp);
        fwrite(centre,sizeof(float),2,fp);
        fwrite(&score,sizeof(int),1,fp);
        fwrite(&next,sizeof(int),1,fp);
        fwrite(&mode,sizeof(int),1,fp);
        fclose(fp);

        printf("Succeed to create new game.\n");
        printf("������Ϸ�ɹ���\n\n");
        Sleep(1000);
        fclose(fp);

        //ѡ����Ϸ�Ѷ�
        mode = choose_mode();

        return 1;
    }
}

//��ӡѡ��
void print_old_or_new(int choice){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch(choice){
        case 0:{
            go_to_x_y(0,5);
            SetConsoleTextAttribute(hConsole,0xf0);
            printf("new game");
            SetConsoleTextAttribute(hConsole,0x0f);
            printf(" prev game");
            SetConsoleTextAttribute(hConsole,0x07);
            printf("\n\n");
            break;
        }
        case 1:{
            go_to_x_y(0,5);
            SetConsoleTextAttribute(hConsole,0x0f);
            printf("new game ");
            SetConsoleTextAttribute(hConsole,0xf0);
            printf("prev game");
            SetConsoleTextAttribute(hConsole,0x07);
            printf("\n\n");
            break;
        }
    }
}

//ѡ��򿪴浵/�����´浵
int new_or_old(){
    int out = 0;
    while(!out){
        system("cls");
        printf("Open previous game or create new game?\n");
        printf("�򿪾��д浵��ʼ����Ϸ?\n\n");
        printf("a for left, d for right, s for choice.\n");
        char key = 'w';
        int choice = 0;
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
        switch(choice){
            case 0:{
                if(create_new())out=1;
                break;
            }
            case 1:{
                if(load_prev())out=1;
                break;
            }
        }
    }
}

//���ع��
void hide_cursor(){
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
}

//������
int main(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    hide_cursor();

    printf("Tetris by Github user Leert2024\n");
    printf("����˹���飬������Github��Leert2024\n\n");

    printf("Gameplay:\n");
    printf("��Ϸ����\n");

    printf("Press s for instant fall.\n");
    printf("��s��ʹ���з����������䣻\n");

    printf("d for right.a for left.\n");
    printf("��d���ƣ���a���ƣ�\n");

    printf("r for clockwise rotation.\n");
    printf("��r˳ʱ����ת��\n");

    printf("l for anticlockwise rotation.\n");
    printf("��l��ʱ����ת��\n");

    printf("p for pause.o for exit.\n");
    printf("��p��ͣ����o������Ϸ��\n\n");

    printf("(Attention: Please set keyboard to English)\n");
    printf("��ע�⣺�뽫���뷨�л�ΪӢ�Ľ�����Ϸ��\n\n");

    printf("Press any key to start...\n");
    printf("���������ʼ��Ϸ����\n");
    _getch();

    //ѡ��ʼ����Ϸ�����������Ϸ
    new_or_old();
    if(mode == 5)mode = choose_mode();
    switch(mode){
    case 1:
        SLEEP = 0.5;HEIGHT = 21;break;
    case 2:
        SLEEP = 0.4;HEIGHT = 20;break;
    case 3:
        SLEEP = 0.3;HEIGHT = 19;break;
    case 4:
        SLEEP = 0.2;HEIGHT = 18;break;
    }

    srand(time(0));

    //��ӡ��Ϸ���
    print_board();
    print_ui(mode);

    print_next(next);

    //��Ϸ�����е�ѭ��
    while(out != 1){
        if(out == 2){//����Ϸ��ͣʱ
            COORD pos={0,3+HEIGHT};
            SetConsoleCursorPosition(hConsole,pos);
            SetConsoleTextAttribute(hConsole,0x07);
            printf("Game paused. Press any key to continue.                   \n");
            _getch();
            SetConsoleCursorPosition(hConsole,pos);
            printf("                                                          ");
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
    printf("Game over! Press any key for exit!\n                                ");
    _getch();
    return 0;
}
