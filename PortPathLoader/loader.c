/*
 * 前の接続情報を引っ張ってくる
*/
#include "loader.h"

// ポートパスを読み込む
int loadPortPath(char *portPath, unsigned char buflen){
    // ファイル開いて
    FILE *fp;
    fp = fopen("/tmp/serterm/portPath", "r");
    if(fp == NULL){
        return errno;
    }

    // パス読んで
    fread(portPath, sizeof(char), buflen, fp);

    // 閉じる!
    fclose(fp);

    return 0;
}

// ポートパスを保存
void savePortPath(char *portPath){
    // ファイル開いて
    FILE *fp;
    system("mkdir -p /tmp/serterm/ > /dev/null");
    fp = fopen("/tmp/serterm/portPath", "w");
    if(fp == NULL){
        printf("File open error: %d\n", errno);
        return;
    }

    // パス書いて
    fwrite(portPath, sizeof(char), strlen(portPath), fp);

    // 保存!
    fclose(fp);
}
