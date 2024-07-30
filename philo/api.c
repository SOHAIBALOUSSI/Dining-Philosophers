#include "philo.h"

int get_meals_eaten(t_philo *philo)
{
    int meals;

    pthread_mutex_lock(&philo->meals_mutex);
    meals = philo->meals_eaten;
    pthread_mutex_unlock(&philo->meals_mutex);
    return (meals);
}

bool is_someone_dead(t_table *table)
{
    bool result;
    
    pthread_mutex_lock(&table->dead_mutex);
    result = table->dead;
    pthread_mutex_unlock(&table->dead_mutex);
    return (result);
}
void set_dead_state(t_table *table, bool state)
{
    pthread_mutex_lock(&table->dead_mutex);
    table->dead = state;
    pthread_mutex_unlock(&table->dead_mutex);
}

void increment_meals_eaten(t_philo *philo)
{
    pthread_mutex_lock(&philo->meals_mutex);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->meals_mutex);
}