/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreis-ma <dreis-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 18:36:57 by dreis-ma          #+#    #+#             */
/*   Updated: 2023/09/28 17:51:45 by dreis-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_forks_2(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		if (i % 2 == 0)
		{
			data->philos[i]->l_fork = data->forks[i];
			if (i == 0)
				data->philos[i]->r_fork = data->forks[data->n_philo - 1];
			else
				data->philos[i]->r_fork = data->forks[i - 1];
		}
		else
		{
			data->philos[i]->l_fork = data->forks[i - 1];
			data->philos[i]->r_fork = data->forks[i];
		}
		i++;
	}
}

void	init_forks(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(t_fork) * data->n_philo + 1);
	while (i < data->n_philo)
	{
		data->forks[i] = malloc(sizeof(t_fork));
		data->forks[i]->id = i;
		data->forks[i]->locked = false;
		pthread_mutex_init(&data->forks[i]->m_fork, NULL);
		i++;
	}
	data->forks[i] = NULL;
	init_forks_2(data);
}

void	init_philos(t_data *data)
{
	int	i;

	i = 0;
	data->philos = malloc(sizeof(t_philo) * data->n_philo + 1);
	while (i < data->n_philo)
	{
		data->philos[i] = malloc(sizeof(t_philo));
		data->philos[i]->id = i;
		data->philos[i]->meals_eaten = 0;
		data->philos[i]->status = 0;
		data->philos[i]->fork_amount = 0;
		data->philos[i]->time_to_die = data->time_die;
		i++;
	}
	data->philos[i] = NULL;
	init_forks(data);
}

void	init_data(t_data *data, char **argv)
{
	data->n_philo = ft_atoi(argv[1]);
	data->time_die = ft_atoi(argv[2]);
	data->time_eat = ft_atoi(argv[3]);
	data->time_sleep = ft_atoi(argv[4]);
	data->curr_id = -1;
	data->dead = false;
	data->philos_full = 0;
	pthread_mutex_init(&data->m_dead, NULL);
	pthread_mutex_init(&data->m_curr_id, NULL);
	pthread_mutex_init(&data->m_philos_full, NULL);
	if (argv[5])
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
	init_philos(data);
}
