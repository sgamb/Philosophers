SRC = main.c	\
	  time_functions.c

all:
	cc $(SRC) && ./a.out 3 1000 600 600 1
