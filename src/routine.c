/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreis-ma <dreis-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 19:06:51 by dreis-ma          #+#    #+#             */
/*   Updated: 2023/09/19 20:38:31 by dreis-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_message(t_data *data, int philo_id, int message_id)
{
	int				timestamp;
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	timestamp = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	pthread_mutex_lock(&data->m_start_time);
	timestamp = timestamp - data->start_time;
	pthread_mutex_unlock(&data->m_start_time);
	pthread_mutex_lock(&data->m_print);
	if (message_id == 0)
		printf("%i %i has taken a fork\n", timestamp, philo_id);
	if (message_id == 1)
		printf("%i %i is eating\n", timestamp, philo_id);
	if (message_id == 2)
		printf("%i %i is sleeping\n", timestamp, philo_id);
	if (message_id == 3)
		printf("%i %i is thinking\n", timestamp, philo_id);
	if (message_id == 4)
		printf("%i %i died\n", timestamp, philo_id);
	pthread_mutex_unlock(&data->m_print);
}

void	p_eat(t_data *data, int p_index)
{
	bool	available;

	available = false;
	pthread_mutex_lock(&data->philos[p_index]->l_fork->m_fork);
	if (data->philos[p_index]->l_fork->locked == false)
	{
		pthread_mutex_lock(&data->philos[p_index]->r_fork->m_fork);
		if (data->philos[p_index]->r_fork->locked == false)
		{
			data->philos[p_index]->r_fork->locked = true;
			data->philos[p_index]->l_fork->locked = true;
			available = true;
		}
		pthread_mutex_unlock(&data->philos[p_index]->r_fork->m_fork);
	}
	pthread_mutex_unlock(&data->philos[p_index]->l_fork->m_fork);
	if (available == true)
	{
		print_message(data, p_index + 1, 1);
		data->philos[p_index]->meals_eaten++;
	}
}

void	p_sleep(t_data *data, int philo_id)
{
	print_message(data, philo_id, 2);
}

void	p_think(t_data *data, int philo_id)
{
	print_message(data, philo_id, 3);
}

bool	check_if_dead(t_data *data)
{
	bool	result;

	pthread_mutex_lock(&data->m_dead);
	result = data->dead;
	pthread_mutex_unlock(&data->m_dead);
	return (result);
}

void	*routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	pthread_mutex_lock(&data->m_curr_id);
	printf("hello, id: %i\n", data->curr_id);
	pthread_mutex_unlock(&data->m_curr_id);
	/*while (1)
	{
		if (check_if_dead(data) == false)
			break ;
		p_eat(data, 3);
	}*/
	return (0);
}