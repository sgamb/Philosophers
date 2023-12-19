SRC = main.c	\
	  time_functions.c

all:
	cc $(SRC) && ./a.out 4 410 200 200 1
