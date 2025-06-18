/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-fons <dda-fons@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:16:53 by dda-fons          #+#    #+#             */
/*   Updated: 2025/06/18 12:20:31 by dda-fons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	create_single_pipe(int **pipes, int index)
{
	pipes[index] = malloc(sizeof(int) * 2);
	if (!pipes[index])
		return (-1);
	if (pipe(pipes[index]) == -1)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}

int	**create_pipes(int num_pipes)
{
	int	**pipes;
	int	i;

	i = 0;
	if (num_pipes <= 0)
		return (NULL);
	pipes = malloc(sizeof(int *) * num_pipes);
	if (!pipes)
		exit(EXIT_FAILURE);
	while (i < num_pipes)
	{
		if (create_single_pipe(pipes, i) == -1)
		{
			if (pipes[i])
				cleanup_pipes(pipes, i);
			else
				cleanup_malloc(pipes, i);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (pipes);
}

void	init_pipeline(int argc, char **argv, t_pipeline *data, int here_doc)
{
	if (here_doc)
	{
		ft_here(argv, data->fd);
		data->fd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
	{
		data->fd[0] = open(argv[1], O_RDONLY);
		if (data->fd[0] == -1)
			perror(argv[1]);
		data->fd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	if (data->fd[1] == -1)
		perror(argv[argc - 1]);
	data->pipes = create_pipes(data->num_cmds - 1);
	data->pids = malloc(sizeof(pid_t) * data->num_cmds);
	if (!data->pids)
	{
		cleanup_pipes(data->pipes, data->num_cmds - 1);
		exit(EXIT_FAILURE);
	}
}

void	execute_pipeline(char **argv, char **envp, t_pipeline *data, int h_d)
{
	int	i;

	i = 0;
	while (i < data->num_cmds)
	{
		data->pids[i] = fork();
		if (data->pids[i] == -1)
			ft_fork_error(data);
		else if (data->pids[i] == 0)
		{
			setup_child(i, data->num_cmds, data->pipes, data->fd);
			cleanup_pipes(data->pipes, data->num_cmds - 1);
			if (h_d)
				ft_execute(argv[i + 3], envp, NULL);
			else
				ft_execute(argv[i + 2], envp, NULL);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	ft_fork_error(t_pipeline *data)
{
	perror("fork");
	close_fds(data->pipes, data->num_cmds - 1, data->fd[0], data->fd[1]);
	cleanup_pipes(data->pipes, data->num_cmds - 1);
	exit(EXIT_FAILURE);
}
