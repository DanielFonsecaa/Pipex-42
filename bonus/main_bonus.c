/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-fons <dda-fons@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 13:13:37 by dda-fons          #+#    #+#             */
/*   Updated: 2025/06/13 15:40:30 by dda-fons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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

void	init_pipeline(int argc, char **argv, t_pipeline *data)
{
	int	num_cmds;

	num_cmds = argc - 3;
	data->pipes = create_pipes(num_cmds - 1);
	data->pids = malloc(sizeof(pid_t) * num_cmds);
	if (!data->pids)
	{
		cleanup_resources(data->pipes, num_cmds - 1, NULL);
		exit(EXIT_FAILURE);
	}
	data->fd[0] = open(argv[1], O_RDONLY);
	if (data->fd[0] == -1)
		perror(argv[1]);
	data->fd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->fd[1] == -1)
		perror(argv[argc - 1]);
}

void	execute_pipeline(int argc, char **argv, char **envp, t_pipeline *data)
{
	int	num_cmds;
	int	i;

	num_cmds = argc - 3;
	i = 0;
	while (i < num_cmds)
	{
		data->pids[i] = fork();
		if (data->pids[i] == -1)
		{
			perror("fork");
			close_fds(data->pipes, num_cmds - 1, data->fd[0], data->fd[1]);
			cleanup_resources(data->pipes, num_cmds - 1, data->pids);
			exit(EXIT_FAILURE);
		}
		else if (data->pids[i] == 0)
		{
			setup_child(i, num_cmds, data->pipes, data->fd);
			cleanup_resources(data->pipes, num_cmds - 1, data->pids);
			ft_execute(argv[i + 2], envp, NULL);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

int	wait_and_get_exit_status(pid_t *pids, int num_cmds)
{
	int	i;
	int	status;
	int	last_exit_status;

	i = 0;
	last_exit_status = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (i == num_cmds - 1)
		{
			if (WIFEXITED(status))
				last_exit_status = WEXITSTATUS(status);
			else
				last_exit_status = 1;
		}
		i++;
	}
	return (last_exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipeline	data;
	int			num_cmds;
	int			last_exit_status;

	if (argc >= 5)
	{
		num_cmds = argc - 3;
		init_pipeline(argc, argv, &data);
		execute_pipeline(argc, argv, envp, &data);
		close_fds(data.pipes, num_cmds - 1, data.fd[0], data.fd[1]);
		last_exit_status = wait_and_get_exit_status(data.pids, num_cmds);
		cleanup_resources(data.pipes, num_cmds - 1, data.pids);
		if (data.fd[1] == -1)
			exit(EXIT_FAILURE);
		exit(last_exit_status);
	}
	ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 cmd3 cmdn file2\n", 2);
	exit(EXIT_FAILURE);
}
