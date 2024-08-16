/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <sait-alo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:19:42 by sait-alo          #+#    #+#             */
/*   Updated: 2024/08/16 20:05:10 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>
# include <limits.h>

# define RED "\033[0;31m"
# define RESET  "\033[0m"
# define MAX_PHILOS 337
# define MIN_TIME 42
# define USE "[nb_of_philos][die time][eat time][sleep time]*[meals]\n"

typedef long	t_time;

typedef struct s_data
{
	t_time	time_to_die;
	t_time	time_to_eat;
	t_time	time_to_sleep;
	int		nb_of_philos;
	int		meals;
}				t_data;

typedef struct s_philo
{
	int					id;
	int					meals_eaten;
	t_time				last_meal;
	bool				is_full;
	pthread_t			thread;
	pthread_mutex_t		last_meal_mutex;
	pthread_mutex_t		meals_mutex;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
}						t_philo;

typedef struct s_table
{
	t_data			*data;
	pthread_t		monitor;
	t_philo			philos[MAX_PHILOS];
	pthread_mutex_t	forks[MAX_PHILOS];
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	dead_mtx;
	pthread_mutex_t	table_mutex;
	pthread_mutex_t	dead_mutex;
	bool			dead;
	t_time			start_time;
}				t_table;

t_table		*get_table(void);

/*		initialize data		*/
int			init_table(t_data *data, int ac, char **av);

/*		Routines	*/
void		*philo_routine(void *pdata);
void		*monitor_routine(void *data);

/*		Utils		*/
void		error(char *msg);
t_time		getcurrtime(void);
void		sleep_ms(t_time ms);
bool		all_philos_full(t_table *table);
void		print_status(t_philo *philo, char *status);

/*		Api			*/
int			get_meals(t_philo *philo);
t_time		lastm(t_philo *philo);
bool		is_someone_dead(t_table *table);
void		set_dead_state(bool state);
bool		is_full(t_philo *philo);
void		set_full(t_philo *philo);

#endif