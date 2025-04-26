/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:59:38 by hugoganet         #+#    #+#             */
/*   Updated: 2025/04/26 16:32:57 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
#include <stdlib.h>
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
	if (argc != 5 && argc != 6)
		return (1);
	config->nb_philo = ft_atoi(argv[1]);
	config->time_to_die = ft_atoi(argv[2]);
	config->time_to_eat = ft_atoi(argv[3]);
	config->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		config->eat_count = ft_atoi(argv[5]);
	else
		config->eat_count = -1;
	if (config->nb_philo <= 0 || config->time_to_die < 0 || config->time_to_eat < 0 || config->time_to_sleep < 0 || (argc == 6 && config->eat_count <= 0))
		return (1);
	return (0);
}
