# variables
includePath = -I ./ -I SerialClient/

# recipe

a.out: main.c client.o
	gcc ${includePath} client.o main.c

client.o: SerialClient/client.h SerialClient/client.c
	gcc ${includePath} -c SerialClient/client.c
