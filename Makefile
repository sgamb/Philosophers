SRC = main.c	\
	  time_functions.c

all:
	cc $(SRC) && ./a.out 4 1000 600 600 1
