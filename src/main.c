/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreis-ma <dreis-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 14:59:26 by dreis-ma          #+#    #+#             */
/*   Updated: 2023/09/19 20:35:52 by dreis-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*Global variables are forbidden!
Static variables are stored in data segment.
Therefore, they are shared by all threads.*/

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
	pthread_mutex_destroy(&data->m_start_time);
	pthread_mutex_destroy(&data->m_dead);
	pthread_mutex_destroy(&data->m_curr_id);
	free(data->philos);
	free(data->forks);
	free(data);
}

/*void	*thread_f(void *vargp)
{
	int						*my_id;
	static int				s;
	static pthread_mutex_t	lock;

	pthread_mutex_lock(&lock);
	my_id = (int *)vargp;
	if (!s)
		s = 0;
	s++;
	printf("Thread ID: %d, Static: %d\n", *my_id, s);
	pthread_mutex_unlock(&lock);
	return (0);
}*/

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
	//pthread_t	thread_id;
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
		init_data(data, argv);
		while (i < ft_atoi(argv[1]))
		{
			data->curr_id = i;
			if (pthread_create(&data->philos[i]->thread, NULL, &routine, data) != 0)
			{
				printf("\033[1;31mError: Thread creation error\n\033[0m");
				// Clear data here and destroy all mutexes
				return (0);
			}
			i++;
		}
		i = 0;
		while (i < ft_atoi(argv[1]))
		{
			pthread_join(data->philos[i]->thread, NULL);
			i++;
		}
		//routine(data);
		free_data(data);
		pthread_exit(NULL);
	}
	else
		printf("\033[1;31mError: Wrong number of arguments\n\033[0m");
	return (0);
}
