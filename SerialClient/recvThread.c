/*
 * 受信スレッド
*/
#include "client.h"

void *recvThread(void *_conf){
    SerialConf conf = *(SerialConf *)_conf;
    bool *endReq = conf.endReq;
    int port = *(conf.port);

    // 受信ループ
    unsigned char buf[512];
    memset(buf, '\0', 512);
    unsigned int cnt = 0;
    while (!(*endReq)){
        int len = read(port, buf, sizeof(buf));
        if(len > 0){
            for(int i = 0; i < len; i++){
                printf("%02X ", buf[i]);
                fflush(stdout);
                cnt++;
                if(cnt%16==0){
                    printf("\n");
                    cnt = 0;
                }
            }
        }
    }
    pthread_exit(NULL);
}
