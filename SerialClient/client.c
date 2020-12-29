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
int selectPorts(int buflen, int bufCount, char *portPath){
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

    // いくつかのコマンドを受け付ける
    /*
     - (数値): スキャン結果の中から選択して開く
     - q: ポート選択UIの終了を要求
     - r: 再スキャンを要求
    */

    int commBuflen = 5;
    char buffer[commBuflen];
    size_t length;
    memset(buffer, '\0', commBuflen);

    // 入力を読んで
    int result = -1, choice = 0;
    while (result == -1){
        while(fgets(buffer, commBuflen, stdin) == NULL || buffer[0] == '\n');
        length = strlen(buffer);

        // コマンドかチェックして
        if(buffer[0] == 'q' || buffer[0] == 'Q'){
            result = REQUIRE_EXIT;
            continue;
        }
        if(buffer[0] == 'r' || buffer[0] == 'r'){
            result = REQUIRE_RETRY;
            continue;
        }

        // どれでもなければスキャン結果の選択とみなし、数値に変換
        choice = atoi(buffer);
        if(choice < 0 || choice >= portsCount){
            continue;
        }
        memcpy(portPath, available[choice], buflen);
        result = 0;
    }

    // ポートを選択していたら割り当てる
    if(result == 0){
        portPath = available[choice];
    }

    return result;
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