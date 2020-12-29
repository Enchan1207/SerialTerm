/*
 * Cでシリアル通信
*/
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "client.h"

int port; // シリアルポート
bool endreq = false, *endReq = &endreq;

void selectPorts(int buflen, int bufCount, char *portPath);
void savePortPath(char *portPath);
int loadPortPath(char *portPath, unsigned char buflen);

// シグナルハンドラ
void signalHandler(int signo){
    *endReq = true; // ほんとはしっかりシグナル番号見るべき
}

int main(int argc, char *argv[]){
    int buflen = 50, bufCount = 10;
    char *portPath;
    portPath = (char *)calloc(sizeof(char), buflen);
    if(portPath == NULL){
        return EXIT_FAILURE;
    }

    // 前回の構成が存在すればそれを開く
    char *prePortPath;
    prePortPath = (char *)calloc(sizeof(char), buflen);
    if(prePortPath == NULL){
        return EXIT_FAILURE;
    }
    if(loadPortPath(prePortPath, buflen) == 0){
        memcpy(portPath, prePortPath, buflen);
    }else{
        // なければポートを選択
        selectPorts(buflen, bufCount, portPath);
    }

    // 開いたポートのパスを/tmp以下に保存しておく
    savePortPath(portPath);

    // 開く
    int baudRate = B115200; // 通信速度
    struct termios tio; // シリアル通信のコンフィグを管理する構造体
    printf("Opening Serial port \033[36m%s\033[0m ...\n", portPath);
    port = openSetialPort(baudRate, portPath, tio);
    if(port == -1){
        printf("\n\033[31mERROR\033[0m couldn't establish connection to serial port %s.\n", portPath);
        return -1;
    }
    printf("[\033[32mSUCCESS\033[0m] connection has been established.\n");

    // SIGINTを受け取る
    signal(SIGINT, signalHandler);

    // 受信スレッドを立てる
    SerialConf conf;
    conf.port = &port;
    conf.endReq = endReq;

    pthread_t rcvThread;
    pthread_create(&rcvThread, NULL, recvThread, &conf);

    // ターミナル
    char buffer[256];
    size_t length;
    memset(buffer, '\0', 256);
    while (!(*endReq)){
        // 入力を読んで
        if (fgets(buffer, 256, stdin) == NULL || buffer[0] == '\n') {
            continue;
        }
        length = strlen(buffer);

        // シリアルに投げつける
        if(write(port, buffer, length) < 0){
            printf("Some error has been occured!: %d\n", errno);
        }
        memset(buffer, '\0', length);
    }

    // ポートを閉じる
    printf("\nClosing Serial port...\n");
    closeSerialPort(port);
    return 0;
}

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

