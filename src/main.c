/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreis-ma <dreis-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 14:59:26 by dreis-ma          #+#    #+#             */
/*   Updated: 2023/09/16 20:15:44 by dreis-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*Global variables are forbidden!
Static variables are stored in data segment.
Therefore, they are shared by all threads.*/

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

int	main(int argc, char **argv)
{
	pthread_t	thread_id;
	int			i;

	i = 0;
	if (argc == 5 || argc == 6)
	{
		if (validate_input(argv) == 0)
			return (0);
		while (i < ft_atoi(argv[1]))
		{
			pthread_create(&thread_id, NULL, thread_f, (void *)&thread_id);
			pthread_join(thread_id, NULL);
			i++;
		}
		pthread_exit(NULL);
	}
	return (0);
}
