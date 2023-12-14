SRC = main.c	\
	  time_functions.c

all:
	cc $(SRC) && ./a.out 3 200 60000 60000 1
