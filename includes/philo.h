/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hganet <hganet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:03:21 by hugoganet         #+#    #+#             */
/*   Updated: 2025/05/13 11:11:14 by hganet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <pthread.h> // thread_t, pthread_mutex_t
#include <sys/time.h> // gettimeofday
#include <stdio.h>
#include <unistd.h> // usleep
#include <stdlib.h> 

//________________________________________________________
//__________________________STRUCTURES_____________________
/**
 * @typedef t_config
 * @brief Stores simulation configuration parameters parsed from arguments.
 *
 * @param nb_philo     The number of philosophers (also the number of forks).
 * @param time_to_die  Time (ms) a philosopher may live without eating.
 * @param time_to_eat  Time (ms) it takes for a philosopher to eat.
 * @param time_to_sleep Time (ms) a philosopher spends sleeping.
 * @param eat_count    Optional: number of times each philosopher must eat. -1 if unspecified.
 * @param stop_simulation Boolean flag to know when to stop the simulation
 * @param start_time   Timestamp when simulation begins (used for logs and monitor_functioning).
 * @param print_mutex  Mutex to synchronize console output.
 * @param death_mutex Mutex to protect access to the stop_simulation flag.
 */
typedef struct s_config
{
	int nb_philo;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int eat_count;
	int stop_simulation;
	long start_time;
	pthread_mutex_t print_mutex;
	pthread_mutex_t death_mutex;
} t_config;

/**
 * @typedef t_philo
 * @brief Represents a single philosopher in the simulation.
 *
 * @param id         Philosopher ID (1-based).
 * @param meals_eaten Counter for how many times the philosopher ate.
 * @param last_meal  Timestamp in ms of the last meal.
 * @param thread     The philosopher's thread.
 * @param left_fork  Pointer to the left fork mutex.
 * @param right_fork Pointer to the right fork mutex.
 * @param config        Pointer to the shared configuration.
 */
typedef struct s_philo
{
	int id;
	int meals_eaten;
	long last_meal;
	pthread_t thread;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	struct s_config *config;
} t_philo;

//______________________________________________________________________________
//__________________________FUNCTIONS___________________________________________
/**
 * @brief Initializes philosophers, forks, and shared config structures.
 *
 * @param config Pointer to parsed configuration.
 * @param philos Pointer to be set to the array of philosophers.
 * @param forks Pointer to be set to the array of fork mutexes.
 * @return int Returns 0 on success, 1 on failure.
 */
int init_simulation(t_config *config, t_philo **philos, pthread_mutex_t **forks);

/**
 * @brief Parses command-line arguments and fills the configuration struct.
 *
 * Validates that each argument is a positive integer (except the optional last one).
 * If validation fails, returns 1. If successful, fills the struct and returns 0.
 *
 * @param argc Number of arguments.
 * @param argv Argument array.
 * @param config Pointer to config struct to populate.
 * @return int 0 on success, 1 on invalid input.
 */
int parse_args(int argc, char **argv, t_config *config);

/**
 * @brief Simulates the philosopher's life cycle.
 *
 * The philosopher loops infinitely (or until death) through:
 * - Taking forks (locking left and right)
 * - Eating (sleeping and logging)
 * - Putting down forks (unlocking)
 * - Sleeping
 * - Thinking
 *
 * @param arg Pointer to t_philo struct cast from void*.
 * @return void* Always returns NULL.
 */
void *philo_life(void *arg);

/**
 * @brief Returns the current timestamp in milliseconds.
 *
 * Uses gettimeofday to measure real time for event logging
 * and death detection.
 *
 * @return long Timestamp in milliseconds.
 */
long get_timestamp_ms(void);

/**
 * @brief Convert a struct timeval in milliseconds.
 *
 * @param tv Time value in struct timeval to be converted in millisecond.
 * @return A long, the time converted in millisecond.
 */
long timeval_to_ms(struct timeval tv);

/**
 * @brief Frees allocated memory and destroys all mutexes.
 *
 * @param config Pointer to the config (holds print mutex).
 * @param philos Pointer to philosopher array to free.
 * @param forks Pointer to fork array to destroy and free.
 */
void cleanup_simulation(t_config *config, t_philo *philos, pthread_mutex_t *forks);

/**
 * @brief Launches a thread for each philosopher.
 *
 * This function creates a thread for each philosopher, passing its
 * struct to the thread routine. Returns 1 on error, 0 on success.
 *
 * @param config Simulation configuration.
 * @param philos Array of philosopher structs.
 * @return int 0 on success, 1 on failure.
 */
int launch_threads(t_config *config, t_philo *philos);

/**
 * @brief monitor_functions philosophers for death and eating counts.
 *
 * Runs in a separate thread. Stops the simulation if any philosopher dies
 * or if all philosophers have eaten enough times.
 *
 * @param arg The array of philosophers (cast from void*).
 * @return void* Always returns NULL.
 */
void *monitor_function(void *arg);

/**
 * @brief Sleeps safely by frequently checking if simulation stopped.
 *
 * Instead of sleeping fully, it checks every 500us if it should stop early.
 *
 * @param config Pointer to the shared configuration.
 * @param time_ms Duration to sleep in milliseconds.
 */
void safe_sleep(t_config *config, long time_ms);

/**
 * @brief Safely checks if simulation should stop.
 *
 * @param config Pointer to shared configuration.
 * @return int 1 if simulation stopped, 0 otherwise.
 */
int is_simulation_stopped(t_config *config);

/**
 * @brief Logs an action for a philosopher with timestamp.
 *
 * This function prints the current timestamp, the philosopher's ID,
 * and a custom message (e.g., "is eating", "has taken a fork").
 * It uses a mutex to prevent overlapping outputs between threads.
 *
 * @param philo Pointer to the philosopher structure.
 * @param msg The message to log (e.g., "is eating", "is sleeping").
 */
void log_action(t_philo *philo, const char *msg);

/**
 * @brief Makes a philosopher take both forks (left then right).
 *
 * The function locks the left fork first. If the simulation is stopped
 * after taking the left fork, it immediately releases it and stops.
 * Otherwise, it locks the right fork and continues.
 *
 * @param philo Pointer to the philosopher structure.
 */
void take_forks(t_philo *philo);

/**
 * @brief Handles the eating process for a philosopher.
 *
 * Updates the philosopher's last meal timestamp and increments
 * the meal counter. Then sleeps for the configured eating duration
 * and finally releases both forks.
 *
 * @param philo Pointer to the philosopher structure.
 */
void eat(t_philo *philo);

/**
 * @brief Handles the sleeping and thinking actions for a philosopher.
 *
 * Logs that the philosopher is sleeping, then sleeps for the configured
 * sleeping time. After waking up, logs that the philosopher is thinking.
 *
 * @param philo Pointer to the philosopher structure.
 */
void sleep_and_think(t_philo *philo);

#endif