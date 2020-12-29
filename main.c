/*
 * Cでシリアル通信
*/
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#include "client.h"

int port; // シリアルポート
bool endreq = false, *endReq = &endreq;

void selectPorts(int buflen, int bufCount, char *portPath);

// シグナルハンドラ
void signalHandler(int signo){
    *endReq = true; // ほんとはしっかりシグナル番号見るべき
}

int main(int argc, char *argv[]){

    // ポートを選択して
    int buflen = 50, bufCount = 10;
    char *portPath;
    portPath = (char *)calloc(sizeof(char), buflen);
    if(portPath == NULL){
        return EXIT_FAILURE;
    }
    selectPorts(buflen, bufCount, portPath);

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


    // 
    while (!(*endReq)){
        /* code */
    }

    // ポートを閉じる
    printf("\nClosing Serial port...\n");
    closeSerialPort(port);
    return 0;
}
