# include "boardop.h"

/**
 * @brief   判断方块是否在图案中
 * @param   block 方块
 * @param   pattern 图案
 * @return  若方块在图案中，返回1，否则返回0
 */
static int if_in(Block_t block, Pattern_t pattern){
    for(int i=0;i<4;i++){
        if (pattern.blocks[i].y == block.y && pattern.blocks[i].x == block.x){
            return 1;
        }
    }
    return 0;
}

/**
 * @brief   设置游戏面板上的方块
 * @param   board 游戏面板指针
 * @param   block 方块
 * @param   type 方块的值
 * @return  若设置成功，返回1，否则返回0
 */
static int set_block(Board_t* board, Block_t block, int type){
    if(block.y < 0 || block.y >= board->height || block.x < 0 || block.x >= board->width){
        return 0;
    }
    if(type < 0 || type > 7){
        return 0;
    }
    board->addr[block.y][block.x] = type;
    return 1;
}

/**
 * @brief   获取游戏面板上的方块
 * @param   board 游戏面板指针
 * @param   block 方块
 * @return  若获取成功，返回方块的值，否则返回0
 */
static int get_block(Board_t* board, Block_t block){
    if(block.y < 0 || block.y >= board->height || block.x < 0 || block.x >= board->width){
        return 0;
    }
    return board->addr[block.y][block.x];
}

/**
 * @brief   将一个指定颜色的方块绘入面板的指定区域
 * @param   board 游戏面板指针
 * @param   block 方块
 * @param   type  颜色类型
 */
static void draw(Board_t* board, Block_t block, int type){
    if (block.y < 0)return;
    set_block(board, block, type);
    Console_GoTo(block.x*2+2, block.y+1);
    Display_SetColor(type);
    printf("  ");
}

/**
 * @brief   左移当前可控图案
 * @param   board 指向游戏面板的指针
 * @param   pattern 指向当前可控图案的指针
 * @return  若左移成功，返回1，否则返回0
 */
int Board_MoveLeft(Board_t* board, Pattern_t* pattern){
    for(int i=0; i<4; i++){
        int x = pattern->blocks[i].x;
        int y = pattern->blocks[i].y;
        if(x - 1 < 0)return 0;
        if(get_block(board, (Block_t){x - 1, y}) != 0 && if_in((Block_t){x - 1, y}, *pattern) == 0){
            return 0;
        }
    }
    for(int i=0; i<4; i++)draw(board, pattern->blocks[i], 0);
    for(int i=0; i<4; i++)pattern->blocks[i].x--;
    for(int i=0; i<4; i++)draw(board, pattern->blocks[i], pattern->type);
    pattern->centre.x--;
    return 1;
}

/**
 * @brief   右移当前可控图案
 * @param   board 指向游戏面板的指针
 * @param   pattern 指向当前可控图案的指针
 * @return  若右移成功，返回1，否则返回0
 */
int Board_MoveRight(Board_t* board, Pattern_t* pattern){
    for(int i=0; i<4; i++){
        int x = pattern->blocks[i].x;
        int y = pattern->blocks[i].y;
        if(x + 1 >= board->width)return 0;
        if(get_block(board, (Block_t){x + 1, y}) != 0 && if_in((Block_t){x + 1, y}, *pattern) == 0){
            return 0;
        }
    }
    for(int i=0; i<4; i++)draw(board, pattern->blocks[i], 0);
    for(int i=0; i<4; i++)pattern->blocks[i].x++;
    for(int i=0; i<4; i++)draw(board, pattern->blocks[i], pattern->type);
    pattern->centre.x++;
    return 1;
}

/**
 * @brief   下移当前可控图案
 * @param   board 指向游戏面板的指针
 * @param   pattern 指向当前可控图案的指针
 * @return  若下移成功，返回1，否则返回0
 */
int Board_MoveDown(Board_t* board, Pattern_t* pattern){
    for(int i=0; i<4; i++){
        int x = pattern->blocks[i].x;
        int y = pattern->blocks[i].y;
        if(y + 1 >= board->height)return 0;
        if(get_block(board, (Block_t){x, y + 1}) != 0 && if_in((Block_t){x, y + 1}, *pattern) == 0){
            return 0;
        }
    }
    for(int i=0; i<4; i++)draw(board, pattern->blocks[i], 0);
    for(int i=0; i<4; i++)pattern->blocks[i].y++;
    for(int i=0; i<4; i++)draw(board, pattern->blocks[i], pattern->type);
    pattern->centre.y++;
    return 1;
}

/**
 * @brief   左旋转当前可控图案
 * @param   board 指向游戏面板的指针
 * @param   pattern 指向当前可控图案的指针
 * @return  若左旋转成功，返回1，否则返回0
 */
int Board_RotateLeft(Board_t* board, Pattern_t* pattern){
    int can_rotate, xs[4], ys[4];

    /* 暂时清空原位置 */
    for(int i = 0; i < 4; i++)set_block(board, pattern->blocks[i], 0);

    /* 坐标变换(计算新位置) */
    for(int i = 0; i < 4; i++){
        xs[i] = pattern->centre.x + pattern->blocks[i].y - pattern->centre.y;
        ys[i] = pattern->centre.y - pattern->blocks[i].x + pattern->centre.x;
    }

    /* 检查新位置是否合法(是否超出面板边界或与其他方块冲突) */
    for(int i = 0; i < 4; i++){
        if (xs[i] < 0 || xs[i] >= board->width || ys[i] >= board->height || get_block(board, (Block_t){xs[i], ys[i]}) != 0){
            can_rotate = 0;
            break;
        }
        can_rotate = 1;
    }

    /* 若新位置合法，则更新图案位置并绘入面板 */
    if(can_rotate){
        for(int i = 0; i < 4; i++){
            draw(board, pattern->blocks[i], 0);
        }
        for(int i = 0; i < 4; i++){
            pattern->blocks[i].x = xs[i];
            pattern->blocks[i].y = ys[i];
            draw(board, pattern->blocks[i], pattern->type);
        }
    }
    
    /* 若新位置不合法，则恢复原位置 */
    else{
        for(int i = 0; i < 4; i++)set_block(board, pattern->blocks[i], pattern->type);
    }
    return can_rotate;
}

/**
 * @brief   右旋转当前可控图案
 * @param   board 指向游戏面板的指针
 * @param   pattern 指向当前可控图案的指针
 * @return  若右旋转成功，返回1，否则返回0
 */
int Board_RotateRight(Board_t* board, Pattern_t* pattern){
    int can_rotate, xs[4], ys[4];
    /* 暂时清空原位置 */
    for(int i = 0; i < 4; i++)set_block(board, pattern->blocks[i], 0);

    /* 坐标变换(计算新位置) */
    for(int i = 0; i < 4; i++){
        xs[i] = pattern->centre.x - pattern->blocks[i].y + pattern->centre.y;
        ys[i] = pattern->blocks[i].x - pattern->centre.x + pattern->centre.y;
    }

    /* 检查新位置是否合法(是否超出面板边界或与其他方块冲突) */
    for(int i = 0; i < 4; i++){
        if (xs[i] < 0 || xs[i] >= board->width || ys[i] >= board->height || get_block(board, (Block_t){xs[i], ys[i]}) != 0){
            can_rotate = 0;
            break;
        }
        can_rotate = 1;
    }

    /* 若新位置合法，则更新图案位置并绘入面板 */
    if(can_rotate){
        for(int i = 0; i < 4; i++){
            draw(board, pattern->blocks[i], 0);
        }
        for(int i = 0; i < 4; i++){
            pattern->blocks[i].x = xs[i];
            pattern->blocks[i].y = ys[i];
            draw(board, pattern->blocks[i], pattern->type);
        }
    }
    
    /* 若新位置不合法，则恢复原位置 */
    else{
        for(int i = 0; i < 4; i++)set_block(board, pattern->blocks[i], pattern->type);
    }
    return can_rotate;
}

/**
 * @brief   尝试进行层的消除
 * @param   board 游戏面板指针
 * @return  能被消除的层数
 */
int Board_Eliminate(Board_t *board){
    int add = 0;
    for(int i = 0; i < board->height; i++){
        int if_score = 1;
        for(int j = 0; j < board->width; j++){
            if (get_block(board, (Block_t){j, i}) == 0){
                if_score = 0;
                break;
            }
        }
        if (if_score){
            add++;
            for(int k=i; k>-1; k--){
                if (k != 0){
                    for (int l=0; l<board->width; l++){
                        draw(board, (Block_t){l, k}, get_block(board, (Block_t){l, k-1}));
                    }
                }else{
                    for (int l=0; l<board->width; l++){
                        draw(board, (Block_t){l, k}, 0);
                    }
                }
            }
        }
    }
    return add;
}
