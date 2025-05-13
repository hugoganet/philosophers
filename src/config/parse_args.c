/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hganet <hganet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:59:38 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/13 11:34:58 by hganet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>

/**
 * @brief Simple atoi that returns -1 on error or overflow.
 */
static int ft_atoi(const char *str)
{
	long result = 0;
	int i = 0;

	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		if (result > INT_MAX)
			return (-1);
		i++;
	}
	if (str[i] != '\0') // contains non-digit characters
		return (-1);
	return ((int)result);
}

int parse_args(int argc, char **argv, t_config *config)
{
	if (argc != 5 && argc != 6) // Check if the number of arguments is valid
		return (0);
	config->nb_philo = ft_atoi(argv[1]); // Store number of philosophers in t_config
	config->time_to_die = ft_atoi(argv[2]); // Store time to die in t_config
	config->time_to_eat = ft_atoi(argv[3]); // Store time to eat in t_config
	config->time_to_sleep = ft_atoi(argv[4]); // Store time to sleep in t_config
	if (argc == 6) // If there are 6 arguments, store the eat count
		config->eat_count = ft_atoi(argv[5]);
	else // If there are only 5 arguments, set eat count to -1
		config->eat_count = -1; 
	if (config->nb_philo <= 0 || config->time_to_die < 0
			|| config->time_to_eat < 0 || config->time_to_sleep < 0
			|| (argc == 6 && config->eat_count <= 0))
		return (0); // Check if any of the arguments are invalid
	return (1);
}
