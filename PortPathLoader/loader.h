/*
 * 前の接続情報を引っ張ってくる
*/
#ifndef _PORT_PATH_LOADER_H_
#define _PORT_PATH_LOADER_H_

/* -------- */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

/* -------- */
void savePortPath(char *portPath);
int loadPortPath(char *portPath, unsigned char buflen);

#endif
