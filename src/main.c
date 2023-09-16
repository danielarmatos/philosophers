/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreis-ma <dreis-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 14:59:26 by dreis-ma          #+#    #+#             */
/*   Updated: 2023/09/16 16:42:46 by dreis-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*Global variables are forbidden!
Static variables are stored in data segment.
Therefore, they are shared by all threads.

mutex */

void *thread_f(void *vargp)
{
	int			*my_id;
	static int	s;

	my_id = (int *)vargp;
	if (!s)
		s = 0;
	s++;
	printf("Thread ID: %d, Static: %d\n", *my_id, s);
	return (0);
}


int	main(int argc, char **argv)
{
	pthread_t	thread_id;
	int			i;

	i = 0;
	if (argc > 4)
	{
		printf("Num of Philosophers: %i\n", atoi(argv[1]));
		while (i < atoi(argv[1]))
		{
			pthread_create(&thread_id, NULL, thread_f, (void *)&thread_id);
			pthread_join(thread_id, NULL);
			i++;
		}
		pthread_exit(NULL);
	}
	return (0);
}
