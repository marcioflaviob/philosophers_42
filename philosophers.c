/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrandao <mbrandao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 23:11:34 by mbrandao          #+#    #+#             */
/*   Updated: 2024/03/03 00:58:28 by mbrandao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*teste()
{
	void *result = NULL;
	printf("Philo initiated.\n");
	sleep(3);
	printf("Finished philo.\n");
	return result;
}

void	create_philos(t_table *table)
{
	int	i;
	t_philo *philos;

	i = 0;
	philos = table->philos;
	if (!philos)
		return ;
	while (i < table->philo_num)
	{
		philos[i].id = i;
		philos[i].forks = table->forks;
		philos[i].r_fork = i;
		philos[i].l_fork = i - 1;
		if (i == 0)
			philos[i].l_fork = (table->philo_num - 1);
		pthread_mutex_init(&philos[i].forks[i], NULL);
		pthread_create(&philos[i].thread, NULL, teste, NULL);
		i++;
	}
}

void	join_philos(t_table *table)
{
	int	i;
	t_philo *philos;

	i = 0;
	philos = table->philos;
	while (i < table->philo_num)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
}


int	main()
{
	t_table table;
	t_philo philos[5];
	pthread_mutex_t forks[5];

	table.philo_num = 5;
	table.time_starve = 800;
	table.time_eat = 200;
	table.time_sleep = 200;
	table.eat_num = 7;
	table.philos = &philos[0];
	table.forks = &forks[0];
	if (table.philo_num > 200)
		return (0);
	create_philos(&table);
	join_philos(&table);
	return (0);
}

