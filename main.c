/*
 * Cでシリアル通信
*/
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>

#include "client.h"

int port; // シリアルポート
bool endReq = false;

void selectPorts(int buflen, int bufCount, char *portPath);

// シグナルハンドラ
void signalHandler(int signo){
    endReq = true; // ほんとはしっかりシグナル番号見るべき
}

int main(int argc, char *argv[]){

    // テキトーにバッファを初期化して
    int buflen = 50, bufCount = 10;
    char *portPath;
    portPath = (char *)calloc(sizeof(char), buflen);
    if(portPath == NULL){
        return EXIT_FAILURE;
    }

    /*
    char *available[buflen];
    for (int i = 0; i < 10; i++){
        available[i] = (char *)calloc(sizeof(char), buflen);
    }

    // ポート探索!
    printf("Discovering serial ports...\n");
    int portsCount = discover(available, buflen, bufCount);
    for(int i = 0; i < portsCount; i++){
        printf("[%d] ", i);
        char tmp = ' ';
        int idx = 0;
        printf("%s\n", available[i]);
    }

    // 結果を選択させて
    int choice = -1;
    while(choice < 0 || choice >= portsCount){
        printf("Which port do you use? ");
        scanf("%d", &choice); // YOU SHOULDN'T USE SCANF
    }
    portPath = available[choice];
    */

    selectPorts(buflen, bufCount, portPath);

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

    // ターミナル
    while (!endReq){

    }

    // ポートを閉じる
    printf("\nClosing Serial port...\n");
    closeSerialPort(port);
    return 0;
}
