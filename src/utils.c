/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreis-ma <dreis-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 20:00:55 by dreis-ma          #+#    #+#             */
/*   Updated: 2023/09/20 20:15:11 by dreis-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	value;
	int	r;

	i = 0;
	value = 1;
	r = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			value = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		r = r * 10 + str[i] - '0';
		i++;
	}
	r = r * value;
	return (r);
}

int	check_if_digit(char **arr)
{
	int	i;
	int	j;

	i = 1;
	while (arr[i])
	{
		j = 0;
		while (arr[i][j])
		{
			if (arr[i][j] >= '0' && arr[i][j] <= '9')
				j++;
			else
				return (0);
		}
		i++;
	}
	return (1);
}

int	get_timestamp()
{
	int				timestamp;
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	timestamp = (current_time.tv_sec * 1000000 + current_time.tv_usec);
	return (timestamp * 0.001);
}