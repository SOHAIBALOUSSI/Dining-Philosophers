NAME = philo_bonus
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = philo_bonus.c init_bonus.c  utils_bonus.c routine_bonus.c
OBJS = $(SRCS:.c=.o)

.c.o :
	@$(CC) $(CFLAGS) -c $< -o $@

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -pthread
 
clean :
	rm -fv $(OBJS)

fclean : clean
	rm -fv $(NAME)

re : fclean all

.PHONY : all clean fclean