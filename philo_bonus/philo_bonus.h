#ifndef PHILO_BONUS
#define PHILO_BONUS

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>
# include <semaphore.h>
# include <fcntl.h>

# ifndef MAX_PHILOS 
	# define MAX_PHILOS 227
# endif /*MAX_PHILOS*/

# define MIN_TIME 42
# define USAGE "Usage:\n./philo [nb_of_philos] [time_to_die] [time_to_eat] [time_to_sleep] *[meals]\n"

typedef long t_time;

typedef struct  s_data
{
	t_time	time_to_die;
	t_time	time_to_eat;
	t_time	time_to_sleep;
	int     nb_of_philos;
	int		meals;
}               t_data;


typedef struct  s_philo
{
	int					id;
	_Atomic int			meals_eaten;
	_Atomic long long	last_meal;
	// pthread_t			thread;
	bool		is_dead;
	sem_t		*last_meal_sem;
	sem_t		*meals_sem;
	sem_t		*left_fork;
	sem_t		*right_fork;
}               t_philo;

typedef struct s_table
{
	t_data			*data;
	// pthread_t		monitor;
	t_philo			philos[MAX_PHILOS];
	sem_t			*forks;
	sem_t			*log_sem;
	sem_t 			*table_sem;
	sem_t			*dead_sem;
	_Atomic bool	dead;
	long long		start_time;
}              t_table;

void pop_error(char *msg);
t_time getcurrtime(void);

int	ft_atoi(char *str);

#endif /* PHILO_BONUS */