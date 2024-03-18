/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrandao <mbrandao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 21:34:24 by mbrandao          #+#    #+#             */
/*   Updated: 2024/03/18 23:12:21 by mbrandao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

long int	get_current_time(void)
{
	long int			time;
	struct timeval		current_time;

	time = 0;
	gettimeofday(&current_time, NULL);
	time = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (time);
}

void	ft_usleep(long int time_in_ms, t_table *table)
{
	long int	start_time;

	start_time = 0;
	start_time = get_current_time();
	while ((get_current_time() - start_time) < time_in_ms)
	{
		if (stop_check(table))
			return ;
		usleep(time_in_ms / 10);
	}
}

int	stop_check(t_table *table)
{
	pthread_mutex_lock(table->stop_lock);
	if (table->stop)
	{
		pthread_mutex_unlock(table->stop_lock);
		return (1);
	}
	pthread_mutex_unlock(table->stop_lock);
	return (0);
}

int	ft_isdigit(int c)
{
	if ((c < 48) || (c > 57))
		return (0);
	else
		return (1);
}

long	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while ((str[i] == ' ') || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		sign = sign * (1 - 2 * (str[i++] == '-'));
	while (ft_isdigit(str[i]))
		result = (result * 10) + (str[i++] - 48);
	if (str[i] != 0 || sign == -1 || result == 0 || result > INT_MAX)
	{
		printf("Error\nInvalid argument(s).\n");
		return (0);
	}
	return (result * sign);
}
