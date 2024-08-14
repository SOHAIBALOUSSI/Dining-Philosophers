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
		pop_error("Error: arguments cannot be set to negative values\n");
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			pop_error("Error: only numbers are allowed\n");
		result = result * 10;
		if (result > 2147483647)
			pop_error("Error: number is too big\n");
		result += str[i] - '0';
		i++;
	}
	return (result);
}