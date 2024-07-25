# ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
#include <sys/time.h>


#define MAX_PHILOS 200
#define MIN_TIME 60

# define USAGE "Usage:\n./philo [nb_of_philos] [time_to_die] [time_to_eat] [time_to_sleep] *[meals]\n"

typedef struct  s_data
{
	int         nb_of_philos;
	int         time_to_die;
	int         time_to_eat;
	int         time_to_sleep;
	int         meals;
}               t_data;

typedef struct  s_fork
{
	pthread_mutex_t mutex;
	bool            is_available;
}               t_fork;

typedef struct  s_philo
{
	pthread_t		thread;
	int				id;
	int				meals_eaten;
	int				last_meal;
	long long		start_time;
	bool			is_dead;
	pthread_mutex_t	mutex;
	t_fork			*left_fork;
	t_fork			*right_fork;
}               t_philo;


typedef struct s_table
{
	t_philo			philos[MAX_PHILOS];
	t_fork			forks[MAX_PHILOS];
	t_data			*data;
	bool			dead;
	pthread_mutex_t	log_mutex;
	long long		start_time;
}              t_table;

t_table *table(void);

# endif