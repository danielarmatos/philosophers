/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreis-ma <dreis-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 14:59:26 by dreis-ma          #+#    #+#             */
/*   Updated: 2023/09/19 18:17:24 by dreis-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*Global variables are forbidden!
Static variables are stored in data segment.
Therefore, they are shared by all threads.*/

void	clear_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		free(data->philos[i]);
		i++;
	}
	free(data->philos);
	free(data);
}

void	test_philos(t_data *data)
{
	int  i;

	i = 0;
	while (i < data->n_philo)
	{
		data->philos[i] = malloc(sizeof(t_philo));
		data->philos[i]->id = i + 1;
		i++;
	}
	i = 0;
	while (i < data->n_philo)
	{
		printf("Philo: %i\n", data->philos[i]->id);
		i++;
	}
}

void	init_data(t_data *data, char **argv)
{
	struct timeval current_time;

	if (gettimeofday(&current_time, NULL) != 0)
	{
		printf("\033[1;31mError getting the current time\n\033[0m");
		return ;
	}
	printf("seconds : %ld\nmicro seconds : %ld\n\n", current_time.tv_sec, current_time.tv_usec);
	printf("%li\n", (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
	printf("%li\n", (current_time.tv_sec * 1000000 + current_time.tv_usec));
	data->n_philo = ft_atoi(argv[1]);
	data->time_die = ft_atoi(argv[2]);
	data->time_eat = ft_atoi(argv[3]);
	data->time_sleep = ft_atoi(argv[4]);
	if (argv[5])
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
	data->philos = malloc(sizeof(t_philo) * data->n_philo + 1);
	//printf("Data:\n%i, %i, %i, %i, %i\n", data->n_philo, data->time_die, data->time_eat, data->time_sleep, data->must_eat);
	test_philos(data);
}

void *thread_f(void *vargp)
{
	int			*my_id;
	static int	s;
	static pthread_mutex_t	lock;

	pthread_mutex_lock(&lock);
	my_id = (int *)vargp;
	if (!s)
		s = 0;
	s++;
	printf("Thread ID: %d, Static: %d\n", *my_id, s);
	pthread_mutex_unlock(&lock);
	return (0);
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

/*void	philosophers(char **argv)
{

}*/

int	main(int argc, char **argv)
{
	pthread_t	thread_id;
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
			pthread_create(&thread_id, NULL, thread_f, (void *)&thread_id);
			pthread_join(thread_id, NULL);
			i++;
		}
		clear_data(data);
		pthread_exit(NULL);
	}
	else
		printf("\033[1;31mError: Wrong number of arguments\n\033[0m");
	return (0);
}
