/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-fons <dda-fons@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:31:22 by dda-fons          #+#    #+#             */
/*   Updated: 2025/06/20 11:12:08 by dda-fons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include "pipex.h"

void	ft_close_fd(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

void	ft_free(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
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

void	cleanup_malloc(int **pipes, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	cleanup_pipes(int **pipes, int count, t_pipeline *data)
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
	free(data->pids);
}
