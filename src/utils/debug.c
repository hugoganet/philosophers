/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hganet <hganet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:42:08 by hganet            #+#    #+#             */
/*   Updated: 2025/05/19 16:43:50 by hganet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_meals_count(t_philo **philos, t_config config)
{
	int	i;
	
	printf("=== Philosopher meal counts ===\n");
	i = 0;
	while (i < config.nb_philo)
	{
		printf("Philosopher %d ate %d time%s\n",
			   (*philos)[i].id,
			   (*philos)[i].meals_eaten,
			   (*philos)[i].meals_eaten == 1 ? "" : "s");
		i++;
	}
}