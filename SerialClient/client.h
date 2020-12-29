/*
 * シリアル通信クライアント
*/
#ifndef _CLIENT_H_
#define _CLIENT_H_

/* -------- */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

/* -------- */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>

/* -------- */
#define REQUIRE_EXIT 1
#define REQUIRE_RETRY 2

/* -------- */
typedef struct serialconf{
    int* port;
    bool* endReq;
} SerialConf;

/* -------- */
int openSetialPort(unsigned int baudRate, char *portPath, struct termios tio);
int closeSerialPort(int serialPort);
int discover(char *available[], int buflen, int bufCount);
void replaceBlank(char *str);
int selectPorts(int buflen, int bufCount, char *portPath);

/* -------- */
void *recvThread(void *_conf);

#endif
