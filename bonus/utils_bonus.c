/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-fons <dda-fons@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:09:29 by dda-fons          #+#    #+#             */
/*   Updated: 2025/06/16 16:23:28 by dda-fons         ###   ########.fr       */
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
	int	i;

	i = 0;
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
	while (i < num_cmds - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	cleanup_pipes(int **pipes, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

int	create_single_pipe(int **pipes, int index)
{
	pipes[index] = malloc(sizeof(int) * 2);
	if (!pipes[index])
		return (-1);
	if (pipe(pipes[index]) == -1)
	{
		perror("pipe");
		free(pipes[index]);
		return (-1);
	}
	return (0);
}
