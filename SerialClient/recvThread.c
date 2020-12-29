/*
 * 受信スレッド
*/
#include "client.h"

void *recvThread(void *_conf){
    SerialConf conf = *(SerialConf *)_conf;
    bool *endReq = conf.endReq;
    int port = *(conf.port);

    // 受信ループ
    char buf[512];
    memset(buf, '\0', 512);
    while (!(*endReq)){
        int len = read(port, buf, sizeof(buf));
        if(len > 0){
            for(int i = 0; i < len; i++){
                printf("%c", buf[i]);
            }
        }
    }
    pthread_exit(NULL);
}
