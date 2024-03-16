/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrandao <mbrandao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 20:21:47 by mbrandao          #+#    #+#             */
/*   Updated: 2024/03/15 23:57:45 by mbrandao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	join_philos(t_philo *philos)
{
	int		i;

	i = 0;
	while (i < philos[0].table->philo_num)
		pthread_join(*(philos[i++].thread), NULL);
}

void	free_forks(t_table *table)
{
	pthread_mutex_t	**forks;
	int				i;

	i = 0;
	forks = table->forks;
	pthread_mutex_destroy(table->stop_lock);
	free(table->stop_lock);
	while (i < table->philo_num)
	{
		pthread_mutex_destroy(forks[i]);
		free(forks[i++]);
	}
	free(forks);
}

void	free_philos(t_philo *philos)
{
	int		i;
	t_table *table;

	table = philos[0].table;
	join_philos(philos);
	free_forks(table);
	i = 0;
	while (i < philos[0].table->philo_num)
	{
		free(philos[i].thread);
		pthread_mutex_destroy(philos[i].eat_lock);
		free(philos[i++].eat_lock);
	}
	free(philos);
}

