/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreis-ma <dreis-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 17:14:19 by dreis-ma          #+#    #+#             */
/*   Updated: 2023/09/28 17:50:41 by dreis-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	check_if_dead(t_data *data)
{
	bool	result;

	pthread_mutex_lock(&data->m_philos_full);
	if (data->philos_full == data->n_philo)
		data->dead = true;
	result = data->dead;
	pthread_mutex_unlock(&data->m_philos_full);
	return (result);
}

void	print_message(t_data *data, int p_index, int message_id)
{
	int	timestamp;

	pthread_mutex_lock(&data->m_dead);
	if (check_if_dead(data) == true)
	{
		pthread_mutex_unlock(&data->m_dead);
		return ;
	}
	timestamp = (get_timestamp(data));
	if (message_id == 0)
		printf("%i %i has taken a fork\n", timestamp, p_index);
	if (message_id == 1)
		printf("%i %i is eating\n", timestamp, p_index);
	if (message_id == 2)
		printf("%i %i is sleeping\n", timestamp, p_index);
	if (message_id == 3)
		printf("%i %i is thinking\n", timestamp, p_index);
	pthread_mutex_unlock(&data->m_dead);
}

void	p_die(t_data *data, int p_index)
{
	int	timestamp;

	pthread_mutex_lock(&data->m_dead);
	if (check_if_dead(data) == true)
	{
		pthread_mutex_unlock(&data->m_dead);
		return ;
	}
	timestamp = get_timestamp(data);
	data->philos[p_index]->status = DEAD;
	data->dead = true;
	printf("%i %i died\n", timestamp, p_index);
	pthread_mutex_unlock(&data->m_dead);
}

void	p_sleep(t_data *data, int p_index)
{
	unsigned int	sleep_time;
	unsigned int	timestamp;

	timestamp = get_timestamp(data);
	if (timestamp >= data->philos[p_index]->time_to_die)
		p_die(data, p_index);
	else
	{
		if (data->philos[p_index]->status != DEAD)
		{
			sleep_time = timestamp + data->time_sleep;
			print_message(data, p_index, 2);
			data->philos[p_index]->status = SLEEPING;
			if (sleep_time < data->philos[p_index]->time_to_die)
				usleep(data->time_sleep * 1000);
			else
			{
				sleep_time = data->philos[p_index]->time_to_die - timestamp;
				usleep(sleep_time * 1000);
				p_die(data, p_index);
			}
		}
	}
}
