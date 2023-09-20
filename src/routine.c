/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreis-ma <dreis-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 19:06:51 by dreis-ma          #+#    #+#             */
/*   Updated: 2023/09/20 21:33:02 by dreis-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	check_if_dead(t_data *data)
{
	bool	result;

	pthread_mutex_lock(&data->m_dead);
	result = data->dead;
	pthread_mutex_unlock(&data->m_dead);
	return (result);
}

void	print_message(t_data *data, int p_index, int message_id)
{
	int	timestamp;

	if (check_if_dead(data) == true)
		return ;
	timestamp = get_timestamp();
	//timestamp = (timestamp - data->start_time) * 0.001;
	timestamp = (timestamp - data->start_time);
	pthread_mutex_lock(&data->m_print);
	if (message_id == 0)
		printf("%i %i has taken a fork\n", timestamp, p_index);
	if (message_id == 1)
		printf("%i %i is eating\n", timestamp, p_index);
	if (message_id == 2)
		printf("%i %i is sleeping\n", timestamp, p_index);
	if (message_id == 3)
		printf("%i %i is thinking\n", timestamp, p_index);
	/*if (message_id == 4)
		printf("%i %i died\n", timestamp, p_index);*/
	pthread_mutex_unlock(&data->m_print);
}

void	p_die(t_data *data, int p_index)
{
	int	timestamp;

	if (check_if_dead(data) == true)
		return ;
	timestamp = get_timestamp();
	timestamp = timestamp - data->start_time;
	data->philos[p_index]->status = DEAD;
	pthread_mutex_lock(&data->m_dead);
	data->dead = true;
	pthread_mutex_unlock(&data->m_dead);
	printf("%i %i died\n", timestamp, p_index);
	exit (0);
}

void	p_sleep(t_data *data, int p_index)
{
	if (get_timestamp() >= data->philos[p_index]->time_to_die)
		p_die(data, p_index);
	else
	{
		if (data->philos[p_index]->status != DEAD)
		{
			print_message(data, p_index, 2);
			data->philos[p_index]->status = SLEEPING;
			if (get_timestamp() + data->time_sleep < data->philos[p_index]->time_to_die)
				usleep(data->time_sleep * 1000);
			else
			{
				usleep(data->philos[p_index]->time_to_die - get_timestamp() * 1000);
				p_die(data, p_index);
			}
		}
	}
}

void	p_think(t_data *data, int p_index)
{
	if (data->philos[p_index]->status != THINKING
		&& data->philos[p_index]->status != DEAD)
	{
		print_message(data, p_index, 3);
		data->philos[p_index]->status = THINKING;
	}
}

void	p_eat_2(t_data *data, int p_index)
{
	int	sleep_time;
	print_message(data, p_index, 1);
	data->philos[p_index]->meals_eaten++;
	data->philos[p_index]->status = EATING;
	if (get_timestamp() + data->time_eat < data->philos[p_index]->time_to_die)
	{
		data->philos[p_index]->time_to_die = get_timestamp() + data->time_die;
		printf("here: %i, %i, %i\n", get_timestamp(), data->time_die, data->philos[p_index]->time_to_die);
		usleep(data->time_eat * 1000);
	}
	else
	{
		printf("==\n");
		sleep_time = data->philos[p_index]->time_to_die - get_timestamp();
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
		if (check_if_dead(data) == true)
			break ;
		p_eat(data, id);
	}
	return (0);
}