/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreis-ma <dreis-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 18:36:57 by dreis-ma          #+#    #+#             */
/*   Updated: 2023/09/19 20:37:56 by dreis-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_forks(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(t_fork) * data->n_philo + 1);
	while (i < data->n_philo)
	{
		data->forks[i] = malloc(sizeof(t_fork));
		data->forks[i]->id = i + 1;
		data->forks[i]->locked = false;
		pthread_mutex_init(&data->forks[i]->m_fork, NULL);
		i++;
	}
	data->forks[i] = NULL;
	i = 0;
	while (i < data->n_philo)
	{
		data->philos[i]->l_fork = data->forks[i];
		if (i == 0)
			data->philos[i]->r_fork = data->forks[data->n_philo - 1];
		else
			data->philos[i]->r_fork = data->forks[i - 1];
		i++;
	}
}

void	init_philos(t_data *data)
{
	int	i;

	i = 0;
	data->philos = malloc(sizeof(t_philo) * data->n_philo + 1);
	while (i < data->n_philo)
	{
		data->philos[i] = malloc(sizeof(t_philo));
		data->philos[i]->id = i + 1;
		data->philos[i]->meals_eaten = 0;
		//data->philos[i]->thread = 0;
		i++;
	}
	data->philos[i] = NULL;
	init_forks(data);
	i = 0;
	while (i < data->n_philo)
	{
		printf("Philo: %i, r_fork: %i, l_fork: %i\n", data->philos[i]->id, data->philos[i]->r_fork->id, data->philos[i]->l_fork->id);
		i++;
	}
}

void	init_data(t_data *data, char **argv)
{
	struct	timeval current_time;

	if (gettimeofday(&current_time, NULL) != 0)
	{
		printf("\033[1;31mError getting the current time\n\033[0m");
		return ;
	}
	printf("seconds : %ld\nmicro seconds : %ld\n\n", current_time.tv_sec, current_time.tv_usec);
	printf("%li\n", (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
	printf("%li\n", (current_time.tv_sec * 1000000 + current_time.tv_usec));
	data->start_time = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	data->n_philo = ft_atoi(argv[1]);
	data->time_die = ft_atoi(argv[2]);
	data->time_eat = ft_atoi(argv[3]);
	data->time_sleep = ft_atoi(argv[4]);
	data->dead = false;
	pthread_mutex_init(&data->m_print, NULL);
	pthread_mutex_init(&data->m_start_time, NULL);
	pthread_mutex_init(&data->m_dead, NULL);
	pthread_mutex_init(&data->m_curr_id, NULL);
	if (argv[5])
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
	init_philos(data);
}
