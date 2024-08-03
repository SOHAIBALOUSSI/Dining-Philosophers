NAME = philo
CC = cc
# CFLAGS = -Wall -Wextra -Werror 
#CFLAGS = -fsanitize=thread #-Wall -Wextra -Werror #

SRCS = philo.c init.c routine.c utils.c api.c

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