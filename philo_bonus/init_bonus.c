#include "philo_bonus.h"


int	ft_atoi(char *str)
{
	int		i;
	long	result;

	i = 0;
	result = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
		i++;
	if (str[i] == '+' && str[i + 1] != '-')
		i++;
	if (str[i] == '-')
		return (pop_error("Error: arguments cannot be set to negative values\n"), -2);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (pop_error("Error: only numbers are allowed\n"), -3);
		result = result * 10;
		if (result > 2147483647)
			return (pop_error("Error: number is too big\n"), -999);
		result += str[i] - '0';
		i++;
	}
	return (result);
}