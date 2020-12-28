# variables
includePath = -I ./ -lncurses 

# recipe

a.out: main.c
	gcc ${includePath} main.c
