NAME = philo
CC = cc
#CFLAGS = -Wall -Wextra -Werror

SRCS = philo.c

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