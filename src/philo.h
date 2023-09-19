/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreis-ma <dreis-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 14:59:18 by dreis-ma          #+#    #+#             */
/*   Updated: 2023/09/19 20:09:56 by dreis-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>

typedef struct s_fork
{
	int				id;
	bool			locked;
	pthread_mutex_t	m_fork;
}		t_fork;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	t_fork			*r_fork;
	t_fork			*l_fork;
	pthread_t		thread;
}		t_philo;

typedef struct s_data
{
	int				n_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				must_eat;
	int				start_time;
	bool			dead;
	int				curr_id;
	t_philo			**philos;
	t_fork			**forks;
	pthread_mutex_t	m_print;
	pthread_mutex_t	m_start_time;
	pthread_mutex_t	m_dead;
	pthread_mutex_t	m_curr_id;
}		t_data;

// Utils:
int		check_if_digit(char **arr);
int		ft_atoi(const char *str);

// Init:
void	init_data(t_data *data, char **argv);

// Routine:
void	*routine(void *arg);

#endif
