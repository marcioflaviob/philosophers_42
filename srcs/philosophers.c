/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrandao <mbrandao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 23:11:34 by mbrandao          #+#    #+#             */
/*   Updated: 2024/03/18 17:35:59 by mbrandao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	*philo_init(t_table **table, t_philo *philo, int *i)
{
	philo->id = *i;
	philo->r_fork = *i;
	philo->l_fork = *i - 1;
	philo->eat_counter = (*table)->eat_num;
	philo->table = *table;
	philo->last_meal = 0;
	philo->is_eating = 0;
	if ((*table)->philo_num == 1)
		philo->l_fork = -1;
	if (*i == 0 && (*table)->philo_num > 1)
		philo->l_fork = ((*table)->philo_num - 1);
	philo->thread = (pthread_t *) malloc (sizeof(pthread_t));
	(*table)->forks[*i] = (pthread_mutex_t *) malloc (sizeof(pthread_mutex_t));
	philo->eat_lock = (pthread_mutex_t *) malloc (sizeof(pthread_mutex_t));
	if (!philo->thread || !(*table)->forks[*i] || !philo->eat_lock)
		return (NULL);
	pthread_mutex_init(philo->eat_lock, NULL);
	pthread_mutex_lock(philo->eat_lock);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(philo->eat_lock);
	pthread_mutex_init((*table)->forks[(*i)++], NULL);
	return (philo);
}

t_philo	*create_philos(t_table *table)
{
	int				i;
	t_philo			*philos;

	i = 0;
	philos = (t_philo *) malloc (table->philo_num * sizeof(t_philo));
	table->stop_lock = (pthread_mutex_t *) malloc (sizeof(pthread_mutex_t));
	table->print_lock = (pthread_mutex_t *) malloc (sizeof(pthread_mutex_t));
	if (!philos || !table->stop_lock || !table->print_lock)
		return (NULL);
	table->stop = 0;
	table->start_time = get_current_time();
	pthread_mutex_init(table->stop_lock, NULL);
	pthread_mutex_init(table->print_lock, NULL);
	while (i < table->philo_num)
		if (!philo_init(&table, &philos[i], &i))
			return (NULL);
	i = 0;
	while (i < table->philo_num)
	{
		pthread_create(philos[i].thread, NULL, &routine, &philos[i]);
		i++;
	}
	return (philos);
}

int	program_start(t_philo *philos)
{
	t_table	*table;

	table = philos[0].table;
	table->thread = (pthread_t *) malloc (sizeof(pthread_t));
	if (!table->thread)
		return (0);
	pthread_create(table->thread, NULL, &maestro, philos);
	return (1);
}

int	init(int argc, char **argv, t_table *table)
{
	pthread_mutex_t	**forks;

	if (argc != 5 && argc != 6)
		return (0);
	table->philo_num = ft_atoi(argv[1]);
	table->time_starve = ft_atoi(argv[2]);
	table->time_eat = ft_atoi(argv[3]);
	table->time_sleep = ft_atoi(argv[4]);
	table->eat_num = -1;
	if (argc == 6)
		table->eat_num = ft_atoi(argv[5]);
	if (table->philo_num > 200)
	{
		printf("Number of philosophers can't be more than 200.\n");
		return (0);
	}
	forks = (pthread_mutex_t **)malloc(table->philo_num
			* sizeof(pthread_mutex_t *));
	if (!forks || !(table->philo_num) || !(table->time_starve)
		|| !(table->time_eat) || !(table->time_sleep) || !(table->philo_num))
		return (0);
	table->forks = forks;
	return (1);
}

int	main(int argc, char **argv)
{
	t_table			table;
	t_philo			*philos;

	if (!init(argc, argv, &table))
		return (0);
	philos = create_philos(&table);
	if (!philos)
		return (0);
	if (program_start(philos))
		pthread_join(*(table.thread), NULL);
	free(table.thread);
	free_philos(philos);
	return (0);
}
