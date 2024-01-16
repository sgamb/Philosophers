SRC = main.c	\
	  parsing_functions.c	\
	  time_functions.c		\
	  init_functions.c

all:
	cc $(SRC) && ./a.out 4 410 200 200 1
