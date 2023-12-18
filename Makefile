SRC = main.c	\
	  time_functions.c

all:
	cc $(SRC) && ./a.out 1 510 200 200 1
