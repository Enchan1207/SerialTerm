# variables
includePath = -I ./

# recipe

a.out: main.c
	gcc ${includePath} main.c
