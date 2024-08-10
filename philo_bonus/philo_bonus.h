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
# include <sys/types.h>
# include <signal.h>
# include <sys/wait.h>

#define RED "\033[0;31m"
#define RESET  "\033[0m"

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
	int					meals_eaten;
	t_time				last_meal;
	pthread_t			monitor;
	bool				is_dead;
	bool				is_full;
}               t_philo;

typedef struct s_table
{
	t_data			*data;
	pthread_t		boss;
	t_philo			philos[MAX_PHILOS];
	pid_t			*pids;
	sem_t			*forks;
	sem_t			*log_sem;
	sem_t 			*table_sem;
	sem_t			*dead_sem;
	sem_t			*last_meal_sem;
	_Atomic bool	dead;
	long long		start_time;
}              t_table;

void pop_error(char *msg);
t_time getcurrtime(void);

int		ft_atoi(char *str);
bool    all_philos_ate_enough(t_table *table);

#endif /* PHILO_BONUS */