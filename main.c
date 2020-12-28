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

// functions
int openSetialPort(unsigned int baudRate, char *portPath, struct termios tio);
int closeSerialPort(int serialPort);

int main(int argc, char *argv[]){

    int port; // シリアルポート
    char *portPath = "/dev/cu.usbserial-DO4VYIOW"; // ポートの場所
    int baudRate = B115200; // 通信速度
    struct termios tio; // シリアル通信のコンフィグを管理する構造体

    printf("Opening Serial port... ");
    port = openSetialPort(baudRate, portPath, tio);
    if(port == -1){
        printf("\n\033[31mERROR\033[0m couldn't establish connection to serial port %s.\n", portPath);
        return -1;
    }
    printf("\033[32mSUCCESS\033[0m.\n");

    // 送受信処理ループ
    char buf[255];
    while(1) {
        int len = read(port, buf, sizeof(buf));
        if (len > 0) {
            for(int i = 0; i < len; i++) {
                printf("%c", buf[i]);
            }
            printf("\n");
        }

        write(port, buf, len);
    }

    // ポートを閉じる
    closeSerialPort(port);
    return 0;
}

// ボーレートとポート名、termios構造体を渡してシリアルポートを開く
int openSetialPort(unsigned int baudRate, char *portPath, struct termios tio){
    // ポートを開く
    int fd = open(portPath, O_RDWR);
    if (fd < 0) {
        return -1;
    }

    // 制御設定
    tio.c_cflag |= CREAD; // 受信を有効化
    tio.c_cflag |= CLOCAL; // フロー制御(RTS, CTS, DTRなど)を無視
    tio.c_cflag |= CS8; // データ長 CS5 CS6 CS7
    tio.c_cflag |= 0; // ストップビット CSTOPB
    tio.c_cflag |= 0; // パリティビット PARENB

    // I/Oのボーレートを設定
    cfsetispeed(&tio, baudRate);
    cfsetospeed(&tio, baudRate);

    cfmakeraw(&tio); // 受け取ったデータに特殊処理を施さず、全てそのまま出力する
    tcsetattr(fd, TCSANOW, &tio); // 設定を反映

    return fd;
}

// シリアルポートを閉じる
int closeSerialPort(int serialPort){
    return close(serialPort);
}
