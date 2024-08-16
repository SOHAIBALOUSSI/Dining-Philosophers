NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=thread

SRCS = data_fetch.c data_fetch2.c init_program.c philosophers.c program_utils.c routines.c

OBJS = $(SRCS:.c=.o)
.c.o :
	@$(CC) $(CFLAGS) -c $< -o $@

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -pthread

clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean