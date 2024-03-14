/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrandao <mbrandao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 23:11:34 by mbrandao          #+#    #+#             */
/*   Updated: 2024/03/14 23:22:06 by mbrandao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	eat(t_philo *philo, t_table *table)
{
	pthread_mutex_lock(table->forks[philo->r_fork]);
	printf("%zu %d has taken a fork.\n", get_current_time(), philo->id);
	if (table->philo_num == 1)
	{
		usleep(table->time_starve);
		pthread_mutex_unlock(table->forks[philo->r_fork]);
		return;
	}
	pthread_mutex_lock(table->forks[philo->l_fork]);
	printf("%zu %d has taken a fork.\n", get_current_time(), philo->id);
	printf("%zu %d is eating.\n", get_current_time(), philo->id);
	pthread_mutex_lock(philo->eat_lock);
	philo->is_eating = 1;
	pthread_mutex_unlock(philo->eat_lock);
	usleep(table->time_eat);
	pthread_mutex_unlock(table->forks[philo->r_fork]);
	pthread_mutex_unlock(table->forks[philo->l_fork]);
	pthread_mutex_lock(philo->eat_lock);
	philo->last_meal = get_current_time();
	philo->eat_counter--;
	philo->is_eating = 0;
	pthread_mutex_unlock(philo->eat_lock);
}

void	sleep_state(t_philo *philo, t_table *table)
{
	printf("%zu %d is sleeping.\n", get_current_time(), philo->id);
	usleep(table->time_sleep);
}

void	think(t_philo *philo)
{
	printf("%zu %d is thinking.\n", get_current_time(), philo->id);
}

// void	*routine()
// {
// 	void *result = NULL;
// 	printf("Philo initiated.\n");
// 	sleep(3);
// 	printf("Finished philo.\n");
// 	return result;
// }


void	*routine(void *void_philo)
{
	t_philo	*philo;
	t_table	*table;

	philo = void_philo;
	table = philo->table;
	while (1)
	{
		eat(philo, table);
		sleep_state(philo, table);
		think(philo);
	}
	return (NULL);
}

int	is_dead(t_philo *philos)
{
	if () //WRITE THE LOGIC TO CHECK IF IT'S DEAD
	// THE TIME NOW MINUS THE TIME OF LAST MEAL HAS TO BE BIGGER THAN THE TIME TO STARVE IN ORDER TO BE DEAD, AND CHECK IF ITS NOT EATING AT THE MOMENT
}

int	all_ate(t_philo *philos)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (i < philos[0].table->philo_num)
	{
		pthread_mutex_lock(philo->eat_lock);
		if (philos[i++].eat_counter <= 0)
			counter++;
		pthread_mutex_unlock(philo->eat_lock);
	}
	if (counter == philos[0].table->philo_num)
		return (1);
	return (0);
}

void	*program(void *ptr)
{
	int		i;
	t_philo	*philos;

	i = 0;
	philos = (t_philo *)ptr;
	while (1)
	{
		
	}
	return (ptr);
}

t_philo	*create_philos(t_table *table)
{
	int				i;
	t_philo			*philos;

	i = 0;
	philos = (t_philo *) malloc (table->philo_num * sizeof(t_philo));
	if (!philos)
		return (NULL);
	while (i < table->philo_num)
	{
		philos[i].id = i;
		philos[i].r_fork = i;
		philos[i].l_fork = i - 1;
		philos[i].eat_counter = table->eat_num;
		philos[i].table = table;
		philos[i].last_meal = 0;
		philos[i].is_eating = 0;
		if (table->philo_num == 1)
			philos[i].l_fork = -1;
		if (i == 0 && table->philo_num > 1)
			philos[i].l_fork = (table->philo_num - 1);
		philos[i].thread = (pthread_t *) malloc (sizeof(pthread_t));
		table->forks[i] = (pthread_mutex_t *) malloc (sizeof(pthread_mutex_t));
		pthread_create(philos[i].thread, NULL, &routine, &philos[i]);
		pthread_mutex_init(philos[i].eat_lock, NULL);
		pthread_mutex_init(table->forks[i++], NULL);
	}
	return (philos);
}

void	join_philos(t_philo *philos)
{
	int		i;

	i = 0;
	while (i < philos[0].table->philo_num)
	{
		pthread_join(*(philos[i].thread), NULL);
		i++;
	}
}

void	program_start(t_philo *philos)
{
	pthread_create(philos[0].table->thread, NULL, &program, philos);
	pthread_mutex_init(philos[0].table->meal_config, NULL);
	pthread_join(philos[0].table->thread, NULL);
}


int	main()
{
	t_table			table;
	pthread_mutex_t	**forks;
	t_philo			*philos;

	table.philo_num = 5;
	forks = (pthread_mutex_t **)malloc(table.philo_num
			* sizeof(pthread_mutex_t *));
	table.time_starve = 800;
	table.time_eat = 200;
	table.time_sleep = 200;
	table.eat_num = 7;
	table.forks = forks;
	if (table.philo_num > 200)
		return (0);
	philos = create_philos(&table);
	join_philos(philos);


	program_start(philos);




	
	free_forks(&table);
	free_philos(philos);
	return (0);
}

