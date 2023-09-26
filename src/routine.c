/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreis-ma <dreis-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 19:06:51 by dreis-ma          #+#    #+#             */
/*   Updated: 2023/09/26 18:35:27 by dreis-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	check_if_dead(t_data *data)
{
	bool	result;

	//pthread_mutex_lock(&data->m_dead);
	result = data->dead;
	//pthread_mutex_unlock(&data->m_dead);
	return (result);
}

void	print_message(t_data *data, int p_index, int message_id)
{
	int	timestamp;

	pthread_mutex_lock(&data->m_dead);
	if (check_if_dead(data) == true)
		return ;
	timestamp = (get_timestamp(data));
	pthread_mutex_lock(&data->m_print);
	if (message_id == 0)
		printf("%i %i has taken a fork\n", timestamp, p_index);
	if (message_id == 1)
		printf("%i %i is eating\n", timestamp, p_index);
	if (message_id == 2)
		printf("%i %i is sleeping\n", timestamp, p_index);
	if (message_id == 3)
		printf("%i %i is thinking\n", timestamp, p_index);
	pthread_mutex_unlock(&data->m_print);
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
	//pthread_mutex_lock(&data->m_dead);
	data->dead = true;
	//pthread_mutex_unlock(&data->m_dead);
	pthread_mutex_lock(&data->m_print);
	printf("%i %i died\n", timestamp, p_index);
	pthread_mutex_unlock(&data->m_print);
	pthread_mutex_unlock(&data->m_dead);
//	exit (0);
}

void	p_sleep(t_data *data, int p_index)
{
	unsigned int	sleep_time;

	if ((unsigned int)get_timestamp(data) >= data->philos[p_index]->time_to_die)
		p_die(data, p_index);
	else
	{
		if (data->philos[p_index]->status != DEAD)
		{
			sleep_time = get_timestamp(data) + data->time_sleep;
		//	printf("sleep: %i %i %u\n", get_timestamp(data), data->time_sleep, sleep_time);
			print_message(data, p_index, 2);
			data->philos[p_index]->status = SLEEPING;
			//printf("sleeping: %u, %i\n", data->philos[p_index]->time_to_die, get_timestamp(data));
		//	printf("here: %u %u\n", sleep_time, data->philos[p_index]->time_to_die);
			if (sleep_time < data->philos[p_index]->time_to_die)
				usleep(data->time_sleep * 1000);
			else
			{
				sleep_time = data->philos[p_index]->time_to_die - get_timestamp(data);
				//printf("Will die while sleeping\n");
				//printf("here: %i\n", data->philos[p_index]->time_to_die - get_timestamp(data));
				usleep(sleep_time * 1000);
				p_die(data, p_index);
			}
		}
	}
}

void	p_think(t_data *data, int p_index)
{
	if ((unsigned int)get_timestamp(data) >= data->philos[p_index]->time_to_die)
	{
		p_die(data, p_index);
		return ;
	}
	if (data->philos[p_index]->status != THINKING
		&& data->philos[p_index]->status != DEAD)
	{
		print_message(data, p_index, 3);
		data->philos[p_index]->status = THINKING;
	}
}

void	p_eat_2(t_data *data, int p_index)
{
	unsigned int	sleep_time;
	unsigned int	x;

	print_message(data, p_index, 1);
	data->philos[p_index]->meals_eaten++;
	data->philos[p_index]->status = EATING;
	data->philos[p_index]->time_to_die = get_timestamp(data) + data->time_die;
	x = get_timestamp(data) + data->time_eat;
	if (x < data->philos[p_index]->time_to_die)
	{
	//	printf("here 01: %i, %i, %u\n", get_timestamp(data), data->time_die, data->philos[p_index]->time_to_die);
		usleep(data->time_eat * 1000);
	}
	else
	{
	//	printf("here 02: %i, %i, %u\n", get_timestamp(data), data->time_eat, data->philos[p_index]->time_to_die);
	//	printf("Will die while eating\n");
		sleep_time = data->philos[p_index]->time_to_die - get_timestamp(data);
		usleep(sleep_time * 1000);
		p_die(data, p_index);
	}
	pthread_mutex_lock(&data->philos[p_index]->l_fork->m_fork);
	data->philos[p_index]->l_fork->locked = false;
	pthread_mutex_unlock(&data->philos[p_index]->l_fork->m_fork);
	pthread_mutex_lock(&data->philos[p_index]->r_fork->m_fork);
	data->philos[p_index]->r_fork->locked = false;
	pthread_mutex_unlock(&data->philos[p_index]->r_fork->m_fork);
	p_sleep(data, p_index);
}

void	p_eat(t_data *data, int p_index)
{
	bool	available;

	available = false;
	if (data->philos[p_index]->l_fork->id != data->philos[p_index]->r_fork->id)
	{
		pthread_mutex_lock(&data->philos[p_index]->l_fork->m_fork);
		if (data->philos[p_index]->l_fork->locked == false)
		{
			pthread_mutex_lock(&data->philos[p_index]->r_fork->m_fork);
			if (data->philos[p_index]->r_fork->locked == false)
			{
				data->philos[p_index]->r_fork->locked = true;
				data->philos[p_index]->l_fork->locked = true;
				print_message(data, p_index, 0);
				print_message(data, p_index, 0);
				available = true;
			}
			pthread_mutex_unlock(&data->philos[p_index]->r_fork->m_fork);
		}
		pthread_mutex_unlock(&data->philos[p_index]->l_fork->m_fork);
	}
	if (available == true)
		p_eat_2(data, p_index);
	else
		p_think(data, p_index);
}

void	*routine(void *arg)
{
	t_data	*data;
	int 	id;

	data = (t_data *)arg;
	pthread_mutex_lock(&data->m_curr_id);
	id = ++data->curr_id;
	//printf("Philo: %i, r_fork: %i, l_fork: %i\n", data->philos[id]->id, data->philos[id]->r_fork->id, data->philos[id]->l_fork->id);
	pthread_mutex_unlock(&data->m_curr_id);
	while (1)
	{
		pthread_mutex_lock(&data->m_dead);
		if (check_if_dead(data) == true)
		{
			pthread_mutex_unlock(&data->m_dead);
			break ;
		}
		pthread_mutex_unlock(&data->m_dead);
		p_eat(data, id);
	}
	return (0);
}