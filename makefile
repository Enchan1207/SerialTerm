# variables
includePath = -I ./ -I SerialClient/ -pthread

# recipe

a.out: main.c SerialClient.dll
	gcc ${includePath} SerialClient.dll main.c

SerialClient.dll: SerialClient/client.h SerialClient/client.c SerialClient/recvThread.c
	gcc ${includePath} -c SerialClient/client.c
	gcc ${includePath} -c SerialClient/recvThread.c
	gcc client.o recvThread.o -fPIC -shared -o SerialClient.dll -pthread

.PHONY: REFLESH
REFLESH:
	rm *.o
	rm *.dll
	rm *.out
