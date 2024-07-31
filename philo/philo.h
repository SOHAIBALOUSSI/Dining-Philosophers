# ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>


# ifndef MAX_PHILOS
# define MAX_PHILOS 300
# endif

#define MIN_TIME 42
# define USAGE "Usage:\n./philo [nb_of_philos] [time_to_die] [time_to_eat] [time_to_sleep] *[meals]\n"

typedef struct  s_data
{
	long long	nb_of_philos;
	long long	time_to_die;
	long long	time_to_eat;
	long long	time_to_sleep;
	int			meals;
}               t_data;


typedef struct  s_philo
{
	int					id;
	int					meals_eaten;
	long long			last_meal;
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
	t_philo			philos[300];
	pthread_mutex_t	forks[300];
	pthread_mutex_t	log_mutex;
	pthread_mutex_t table_mutex;
	pthread_mutex_t dead_mutex;
	bool	dead;
	long long		start_time;
}              t_table;

t_table *get_table(void);

/*		initialize data	*/
void	init_table(t_data *data, int ac, char **av);

/*		Routines		*/
void	*philo_routine(void *pdata);
void	*monitor_routine(void *data);

/*		utils		*/
void		error(char *msg);
long long	getcurrtime(void);
void		sleep_ms(long long ms);
bool    	all_philos_ate_enough(t_table *table);
void		print_status(t_philo *philo, char *status);

/* Api*/
bool 	is_someone_dead(t_table *table);
void 	set_dead_state(t_table *table, bool state);
void 	increment_meals_eaten(t_philo *philo);
int		get_meals_eaten(t_philo *philo);
long long get_last_meal(t_philo *philo);

# endif