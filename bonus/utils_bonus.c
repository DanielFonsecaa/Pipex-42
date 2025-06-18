/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-fons <dda-fons@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:09:29 by dda-fons          #+#    #+#             */
/*   Updated: 2025/06/18 13:39:45 by dda-fons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	*ft_get_path(char **envp, char *cmd)
{
	char	**full_path;
	char	*half_path;
	char	*path;
	int		i;

	i = 0;
	if (!envp || !cmd)
		return (NULL);
	while (ft_strnstr(envp[i], "PATH=", 5) == 0)
		i++;
	full_path = ft_split(envp[i] + 5, ':');
	i = 0;
	while (full_path[i])
	{
		half_path = ft_strjoin(full_path[i], "/");
		path = ft_strjoin(half_path, cmd);
		free(half_path);
		if (access(path, F_OK | X_OK) == 0)
			return (ft_free(full_path), path);
		free(path);
		i++;
	}
	ft_free(full_path);
	return (NULL);
}

void	ft_execute(char *argv, char **envp, int *fd)
{
	char	*path;
	char	**cmd;

	cmd = ft_split(argv, ' ');
	path = ft_get_path(envp, cmd[0]);
	if (!path)
	{
		ft_free(cmd);
		ft_putstr_fd("command not found", 2);
		if (fd)
			ft_close_fd(fd);
		exit(EXIT_NOTFOUND);
	}
	if (execve(path, cmd, envp) == -1)
	{
		ft_free(cmd);
		if (fd)
			ft_close_fd(fd);
		exit(EXIT_FAILURE);
	}
}

void	setup_child(int index, int num_cmds, int **pipes, int *fd)
{
	if (index == 0)
	{
		if (fd[0] != -1)
		{
			dup2(fd[0], STDIN_FILENO);
			if (fd[0] > 2)
				close(fd[0]);
		}
		else
			close(STDIN_FILENO);
	}
	else
		dup2(pipes[index - 1][0], STDIN_FILENO);
	if (index == num_cmds - 1 && fd[1] != -1)
	{
		dup2(fd[1], STDOUT_FILENO);
		if (fd[1] > 2)
			close(fd[1]);
	}
	else if (index != num_cmds - 1)
		dup2(pipes[index][1], STDOUT_FILENO);
	close_fds(pipes, num_cmds - 1, -1, -1);
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

void	ft_here(char **argv, int *fd)
{
	char	*limiter;
	char	*line;
	int		pipe_fd[2];

	limiter = argv[2];
	if (pipe(pipe_fd) == -1)
		exit(EXIT_FAILURE);
	while (1)
	{
		ft_putstr_fd("heredoc> ", 1);
		line = get_next_line(0);
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& (line[ft_strlen(limiter)] == '\n'
				|| line[ft_strlen(limiter) + 1] == '\0'))
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		free(line);
	}
	close(pipe_fd[1]);
	fd[0] = pipe_fd[0];
}
