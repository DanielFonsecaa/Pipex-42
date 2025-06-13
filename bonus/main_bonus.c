/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-fons <dda-fons@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 13:13:37 by dda-fons          #+#    #+#             */
/*   Updated: 2025/06/13 14:36:52 by dda-fons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	setup_child(int index, int num_cmds, int **pipes, int fd_in, int fd_out);
int	**create_pipes(int num_pipes);
void	close_fds(int **pipes, int num_pipes, int fd_in, int fd_out);
void	cleanup_resources(int **pipes, int num_pipes, pid_t *pids);


int	main(int argc, char **argv, char **envp)
{
	int		fd_in;
	int		fd_out;
	int		i;
	pid_t	*pids;
	int		**pipes;
	int		num_cmds;
	
	if (argc >= 5)
	{
		num_cmds = argc - 3;
		pipes = create_pipes(num_cmds - 1);
		pids = malloc(sizeof(pid_t) * num_cmds);
		if (!pids)
		{
			cleanup_resources(pipes, num_cmds - 1, NULL);
			exit(EXIT_FAILURE);
		}
		i = 0;
		fd_in = open(argv[1], O_RDONLY);
		if (fd_in == -1)
		{
			perror(argv[1]);
		}
		fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd_out == -1)
		{
			perror(argv[argc - 1]);
			//if (fd_in != -1)
			//	close(fd_in);
			//cleanup_resources(pipes, num_cmds - 1, pids);
			//exit(EXIT_FAILURE);
		}
		while (i < num_cmds)
		{
			pids[i] = fork();
			if (pids[i] == -1)
			{
				perror("fork");
				close_fds(pipes, num_cmds - 1, fd_in, fd_out);
				cleanup_resources(pipes, num_cmds - 1, pids);
				exit(EXIT_FAILURE);
			}
			else if (pids[i] == 0)
			{
				setup_child(i, num_cmds, pipes, fd_in, fd_out);
				cleanup_resources(pipes, num_cmds - 1, pids);
				ft_execute(argv[i + 2], envp, NULL);
				exit(EXIT_FAILURE);
			}
			i++;
		}
		close_fds(pipes, num_cmds - 1, fd_in, fd_out);
		i = 0;
		int last_exit_status = 0;
		while (i < num_cmds)
		{
			int status;
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
		cleanup_resources(pipes, num_cmds - 1, pids);
		if (fd_out == -1)
			exit(EXIT_FAILURE);
		exit(last_exit_status); 
	}
	ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 cmd3 cmdn file2\n", 2);
	exit(EXIT_FAILURE);
}

void	setup_child(int index, int num_cmds, int **pipes, int fd_in, int fd_out)
{
	int	i = 0;

	if (index == 0)
	{
		if (fd_in != -1)
			dup2(fd_in, STDIN_FILENO);
		else
			close(STDIN_FILENO);
	}
	else
		dup2(pipes[index - 1][0], STDIN_FILENO);	
	if (index == num_cmds - 1)
	{
		if (fd_out != -1)
			dup2(fd_out, STDOUT_FILENO);
	}
	else
		dup2(pipes[index][1], STDOUT_FILENO);

	while (i < num_cmds - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
	if (fd_in > 2)
		close(fd_in);
	if (fd_out > 2)
		close(fd_out);
}

int	**create_pipes(int num_pipes)
{
	int	**pipes;
	int	i;

	if (num_pipes <= 0)
		return (NULL);

	i = 0;
	pipes = malloc(sizeof(int*) * num_pipes);
	if (!pipes)
		exit(EXIT_FAILURE);

	while (i < num_pipes)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			while (--i >= 0)
				free(pipes[i]);
			free(pipes);
			exit(EXIT_FAILURE);
		}
		
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			free(pipes[i]);
			while (--i >= 0)
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
				free(pipes[i]);
			}
			free(pipes);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (pipes);
}

void	close_fds(int **pipes, int num_pipes, int fd_in, int fd_out)
{
	int	i;

	i = 0;
	while (i < num_pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
	if (fd_in != -1)
		close(fd_in);
	if (fd_out != -1)
		close(fd_out);
}

void	cleanup_resources(int **pipes, int num_pipes, pid_t *pids)
{
	int	i;

	if (pipes)
	{
		i = 0;
		while (i < num_pipes)
		{
			if (pipes[i])
				free(pipes[i]);
			i++;
		}
		free(pipes);
	}
	if (pids)
		free(pids);
}
