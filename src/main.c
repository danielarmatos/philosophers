/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreis-ma <dreis-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 14:59:26 by dreis-ma          #+#    #+#             */
/*   Updated: 2023/09/26 16:27:53 by dreis-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_data(t_data *data)
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
	pthread_mutex_destroy(&data->m_print);
	pthread_mutex_destroy(&data->m_dead);
	pthread_mutex_destroy(&data->m_curr_id);
	free(data->philos);
	free(data->forks);
	free(data);
}

int	validate_input(char **argv)
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

int	main(int argc, char **argv)
{
	int			i;
	t_data		*data;

	i = 0;
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
		while (i < ft_atoi(argv[1]))
		{
			if (pthread_create(&data->philos[i]->thread, NULL, &routine, data) != 0)
			{
				printf("\033[1;31mError: Thread creation error\n\033[0m");
				// Clear data here and destroy all mutexes
				return (0);
			}
			usleep(10);
			i++;
		}
		i = 0;
		while (i < ft_atoi(argv[1]))
		{
			pthread_join(data->philos[i]->thread, NULL);
			i++;
		}
		free_data(data);
	}
	else
		printf("\033[1;31mError: Wrong number of arguments\n\033[0m");
	return (0);
}
