# ifndef MAIN_H
# define MAIN_H

# define WIDTH 10       // 游戏区域宽度
# define MAX_HEIGHT 20  // 游戏区域最大高度

/* 面板 */
typedef struct{
    char addr[MAX_HEIGHT][WIDTH];  // 面板内容
    int width;  // 面板宽度
    int height; // 面板高度
}Board_t;

/* 单个方块坐标 */
typedef struct{
    int x;    // 横坐标
    int y;    // 纵坐标
}Block_t;

/* 图案 */
typedef struct{
    int type;           // 类型
    Block_t blocks[4];  // 含有的4个方块的坐标
    Block_t centre;    // 中心坐标
}Pattern_t;

/* 游戏句柄 */
typedef struct{
    Board_t board;      // 游戏面板
    Pattern_t pattern;  // 当前可控图案
    int score;          // 当前得分
    int difficulty;     // 游戏难度
    int next_type;      // 下一个图案类型
}Game_t;

# endif