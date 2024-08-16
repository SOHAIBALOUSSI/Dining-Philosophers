NAME = philo_bonus
CC = cc
CFLAGS = -fsanitize=thread -Wall -Wextra -Werror 

SRCS = philo_bonus.c init_bonus.c  utils_bonus.c routine_bonus.c
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