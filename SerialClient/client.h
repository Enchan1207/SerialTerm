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

/* -------- */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>

/* -------- */
int openSetialPort(unsigned int baudRate, char *portPath, struct termios tio);
int closeSerialPort(int serialPort);
int discover(char *available[], int buflen, int bufCount);
void replaceBlank(char *str);
void selectPorts(int buflen, int bufCount, char *portPath);

#endif
