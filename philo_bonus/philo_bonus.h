#ifndef PHILO_BONUS
#define PHILO_BONUS

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>

# ifndef MAX_PHILOS
# define MAX_PHILOS _SC_THREAD_THREADS_MAX
# endif

#define MIN_TIME 42
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
	bool				is_dead;
	pthread_t			thread;
	pthread_mutex_t		last_meal_mutex;
	pthread_mutex_t		meals_mutex;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
}               t_philo;

typedef struct s_table
{
	t_data			*data;
	pthread_t		monitor;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t table_mutex;
	pthread_mutex_t dead_mutex;
	_Atomic bool	dead;
	long long		start_time;
}              t_table;

void pop_error(char *msg);
t_time getcurrtime(void);

int	ft_atoi(char *str);

#endif