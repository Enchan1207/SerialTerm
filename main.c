/*
 * Cでシリアル通信
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>

// functions
int openSetialPort(unsigned int baudRate, char *portPath, struct termios tio);
int closeSerialPort(int serialPort);
int discover(char *available[], int buflen, int bufCount);
void replaceBlank(char *str);

int port; // シリアルポート
bool endReq = false;

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
    char *available[buflen];
    for (int i = 0; i < 10; i++){
        available[i] = (char *)calloc(sizeof(char), buflen);
    }

    // ポート探索!
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
    if(signal(SIGINT, signalHandler) == SIG_ERR){
        printf("STOP ERROR!\n");
    }

    // ターミナル
    while (!endReq){

    }

    // ポートを閉じる
    printf("\nClosing Serial port...\n");
    closeSerialPort(port);
    return 0;
}

// ボーレートとポート名、termios構造体を渡してシリアルポートを開く
int openSetialPort(unsigned int baudRate, char *portPath, struct termios tio){
    // ポートを開く
    printf("Open port\n");
    int fd = open(portPath, O_RDWR);
    if(fd < 0){
        return -1;
    }

    // 制御設定
    printf("Condigure settings\n");
    tio.c_cflag |= CREAD;  // 受信を有効化
    tio.c_cflag |= CLOCAL; // フロー制御(RTS, CTS, DTRなど)を無視
    tio.c_cflag |= CS8;    // データ長 CS5 CS6 CS7
    tio.c_cflag |= 0;      // ストップビット CSTOPB
    tio.c_cflag |= 0;      // パリティビット PARENB

    // I/Oのボーレートを設定
    printf("Apply...\n");
    cfsetispeed(&tio, baudRate);
    cfsetospeed(&tio, baudRate);

    cfmakeraw(&tio);              // 受け取ったデータに特殊処理を施さず、全てそのまま出力する
    tcsetattr(fd, TCSANOW, &tio); // 設定を反映

    return fd;
}

// シリアルポートを閉じる
int closeSerialPort(int serialPort){
    return close(serialPort);
}

// デバイス探索
int discover(char *available[], int buflen, int bufCount){
    int availableCount = 0;

    // ls -1 /dev/tty.*
    FILE *fp;
    fp = popen("ls -1 /dev/cu.*", "r");
    if(fp == NULL){
        return 0;
    }
    char buffer[buflen];
    while(!feof(fp) && availableCount < bufCount){
        fgets(buffer, buflen, fp);
        memcpy(available[availableCount], buffer, buflen);
        replaceBlank(available[availableCount]);
        availableCount++;
    }
    pclose(fp);

    return availableCount - 1;
}

// 改行を空白に置き換える
void replaceBlank(char *str){
    char *tmp = str;
    while (*tmp != '\0'){
        if(*tmp == '\n'){
            *tmp = '\0';
        }
        tmp++;
    }
}
