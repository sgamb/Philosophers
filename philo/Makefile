SRC = main.c	\
	  parsing_functions.c	\
	  time_functions.c		\
	  init_functions.c		\
	  routine_functions.c	\
	  tracker_functions.c

OBJ = $(SRC:.c=.o)

NAME = philo

all: $(NAME)

%.o: %.c
	cc -Wall -Werror -Wextra -c $^ -o $@

$(NAME): $(OBJ)
	cc -Wall -Werror -Wextra $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean
	$(MAKE) all
