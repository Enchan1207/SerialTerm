/*
 * Cでシリアル通信
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define SERIAL_PORT "/dev/cu.usbserial-DO4VYIOW"

int main(int argc, char *argv[]){

    int fd;
    struct termios tio; // シリアル通信のコンフィグを管理する構造体
    int baudRate = B115200; // 通信速度

    // ポートを開く
    printf("Opening Serial port...\n");
    fd = open(SERIAL_PORT, O_RDWR);
    if (fd < 0) {
        printf("\033[31mERROR\033[0m: couldn't establish serial-port connection.\n");
        return -1;
    }
    printf("\033[32mSUCCESS\033[0m: open serial port\n");

    tio.c_cflag += CREAD; // 受信
    tio.c_cflag += CLOCAL; // RTS/CTS制御はしない

    // 8N1
    tio.c_cflag += CS8; //8bit
    tio.c_cflag += 0; // 1 stop bit
    tio.c_cflag += 0; // non parity

    // I/Oのボーレートを設定
    cfsetispeed( &tio, baudRate );
    cfsetospeed( &tio, baudRate );

    cfmakeraw(&tio);                    // RAWモード
    tcsetattr( fd, TCSANOW, &tio );     // デバイスに設定を行う

    printf("Serial port configured\n");

    // 送受信処理ループ
    char buf[255];
    while(1) {
        int len = read(fd, buf, sizeof(buf));
        if (len > 0) {
            for(int i = 0; i < len; i++) {
                printf("%c", buf[i]);
            }
            printf("\n");
        }

        write(fd, buf, len);
    }

    // ポートを閉じる
    close(fd);
    return 0;
}
