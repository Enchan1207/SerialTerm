/*
 * シリアル通信クライアント
*/
#include "client.h"

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

// 改行を\0に置き換える
void replaceBlank(char *str){
    char *tmp = str;
    while (*tmp != '\0'){
        if(*tmp == '\n'){
            *tmp = '\0';
        }
        tmp++;
    }
}