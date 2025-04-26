/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:23:30 by hugoganet         #+#    #+#             */
/*   Updated: 2025/04/26 18:23:56 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int launch_threads(t_config *config, t_philo *philos)
{
	int i;

	i = 0;
	while (i < config->nb_philo)
	{
		if (pthread_create(&philos[i].thread, NULL, philo_life, &philos[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}