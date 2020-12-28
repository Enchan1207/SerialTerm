# variables
includePath = -g -O0 -I ./

# recipe

a.out: main.c
	gcc ${includePath} main.c
