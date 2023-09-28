/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreis-ma <dreis-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 14:59:26 by dreis-ma          #+#    #+#             */
/*   Updated: 2023/09/28 17:46:20 by dreis-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	free_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		pthread_mutex_destroy(&data->forks[i]->m_fork);
		free(data->forks[i]);
		free(data->philos[i]);
		i++;
	}
	pthread_mutex_destroy(&data->m_dead);
	pthread_mutex_destroy(&data->m_curr_id);
	pthread_mutex_destroy(&data->m_philos_full);
	free(data->philos);
	free(data->forks);
	free(data);
}

static int	validate_input(char **argv)
{
	int	i;
	int	n;

	if (check_if_digit(argv) == 0)
		return (0);
	i = 1;
	while (argv[i])
	{
		n = ft_atoi(argv[i]);
		if (n < 0)
			return (0);
		if (n == 0 && i != 5)
			return (0);
		i++;
	}
	return (1);
}

static void	philo(char **argv, t_data *data)
{
	int	i;

	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		if (pthread_create(&data->philos[i]->thread, NULL, &routine, data) \
				!= 0)
			break ;
		i++;
	}
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		if (pthread_join(data->philos[i]->thread, NULL) != 0)
			break ;
		i++;
	}
	free_data(data);
}

int	main(int argc, char **argv)
{
	t_data		*data;

	if (argc == 5 || argc == 6)
	{
		if (validate_input(argv) == 0)
		{
			printf("\033[1;31mError: Invalid input\n\033[0m");
			return (0);
		}
		data = malloc(sizeof(t_data));
		data->start_time = get_start_timestamp();
		init_data(data, argv);
		philo(argv, data);
	}
	else
		printf("\033[1;31mError: Wrong number of arguments\n\033[0m");
	return (0);
}
