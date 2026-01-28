# include "fileio.h"

/**
 * @brief   读取游戏信息
 * @param   game 游戏句柄指针
 * @return  成功返回1，失败返回0
 */
int FILEIO_ReadInfo(Game_t *game){
    if(game == NULL){
        return 0;
    }
    FILE *fp;
    if((fp = fopen("prev.dat","rb"))==NULL){
        return 0;
    }
    fread(game,sizeof(Game_t),1,fp);
    fclose(fp);
    return 1;
}

/**
 * @brief   写入游戏信息
 * @param   game 游戏句柄
 * @return  成功返回1，失败返回0
 */
int FILEIO_WriteInfo(Game_t game){
    FILE *fp;
    if((fp = fopen("prev.dat","wb"))==NULL){
        return 0;
    }
    fwrite(&game,sizeof(Game_t),1,fp);
    fclose(fp);
    return 1;
}
