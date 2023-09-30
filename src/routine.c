/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreis-ma <dreis-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 19:06:51 by dreis-ma          #+#    #+#             */
/*   Updated: 2023/09/30 17:34:58 by dreis-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

static void	p_eat_3(t_data *data, int p_index)
{
	pthread_mutex_lock(&data->philos[p_index]->l_fork->m_fork);
	data->philos[p_index]->l_fork->locked = false;
	pthread_mutex_unlock(&data->philos[p_index]->l_fork->m_fork);
	pthread_mutex_lock(&data->philos[p_index]->r_fork->m_fork);
	data->philos[p_index]->r_fork->locked = false;
	pthread_mutex_unlock(&data->philos[p_index]->r_fork->m_fork);
	data->philos[p_index]->fork_amount = 0;
	data->philos[p_index]->has_l_fork = false;
	data->philos[p_index]->has_r_fork = false;
	if (data->philos[p_index]->meals_eaten == data->must_eat)
	{
		pthread_mutex_lock(&data->m_philos_full);
		data->philos_full++;
		pthread_mutex_unlock(&data->m_philos_full);
	}
	p_sleep(data, p_index);
	p_think(data, p_index);
	usleep(300);
}

static void	p_eat_2(t_data *data, int p_index)
{
	unsigned int	sleep_time;
	unsigned int	x;
	unsigned int	timestamp;

	if (data->philos[p_index]->has_l_fork == true
		&& data->philos[p_index]->has_r_fork == true)
	{
		print_message(data, p_index, 1);
		timestamp = get_timestamp(data);
		data->philos[p_index]->status = EATING;
		data->philos[p_index]->time_to_die = timestamp + data->time_die;
		x = timestamp + data->time_eat;
		if (x <= data->philos[p_index]->time_to_die)
			usleep(data->time_eat * 1000);
		else
		{
			sleep_time = data->philos[p_index]->time_to_die - timestamp;
			usleep(sleep_time * 1000);
			p_die(data, p_index);
		}
		data->philos[p_index]->meals_eaten++;
		p_eat_3(data, p_index);
	}
	else
		p_think(data, p_index);
}

void	p_eat(t_data *data, int p_index)
{
	if (data->philos[p_index]->l_fork->id == data->philos[p_index]->r_fork->id)
		one_p_eat(data, p_index);
	else
	{
		pthread_mutex_lock(&data->philos[p_index]->l_fork->m_fork);
		if (data->philos[p_index]->l_fork->locked == false)
		{
			data->philos[p_index]->l_fork->locked = true;
			data->philos[p_index]->has_l_fork = true;
			print_message(data, p_index, 0);
		}
		pthread_mutex_unlock(&data->philos[p_index]->l_fork->m_fork);
		pthread_mutex_lock(&data->philos[p_index]->r_fork->m_fork);
		if (data->philos[p_index]->has_l_fork == true
			&& data->philos[p_index]->r_fork->locked == false)
		{
			data->philos[p_index]->r_fork->locked = true;
			data->philos[p_index]->has_r_fork = true;
			print_message(data, p_index, 0);
		}
		pthread_mutex_unlock(&data->philos[p_index]->r_fork->m_fork);
	}
	p_eat_2(data, p_index);
}

void	*routine(void *arg)
{
	t_data	*data;
	int		id;

	data = (t_data *)arg;
	pthread_mutex_lock(&data->m_curr_id);
	id = ++data->curr_id;
	pthread_mutex_unlock(&data->m_curr_id);
	if (id % 2 == 0)
	{
		p_think(data, id);
		usleep(5000);
	}
	while (1)
	{
		p_eat(data, id);
		pthread_mutex_lock(&data->m_dead);
		if (check_if_dead(data) == true)
		{
			pthread_mutex_unlock(&data->m_dead);
			break ;
		}
		pthread_mutex_unlock(&data->m_dead);
	}
	return (0);
}
