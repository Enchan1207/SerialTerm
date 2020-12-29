/*
 * シリアル通信クライアント
*/
#include "client.h"

// ボーレートとポート名、termios構造体を渡してシリアルポートを開く
int openSetialPort(unsigned int baudRate, char *portPath, struct termios tio){
    // ポートを開く
    printf("Open port\n");
    int fd = open(portPath, O_RDWR | O_NONBLOCK);
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
    tio.c_lflag &= ~(ICANON); // カノニカルモード無効化
    tio.c_cc[VMIN] = 0;
    tio.c_cc[VTIME] = 0;

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

// デバイスを探索し、選択UIを表示 結果をchar*で返す
void selectPorts(int buflen, int bufCount, char *portPath){
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
        scanf("%1d", &choice); // YOU SHOULDN'T USE SCANF
    }
    memcpy(portPath, available[choice], buflen);

    // 戻す
    portPath = available[choice];
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