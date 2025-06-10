/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-fons <dda-fons@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:09:29 by dda-fons          #+#    #+#             */
/*   Updated: 2025/06/10 18:14:28 by dda-fons         ###   ########.fr       */
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
		ft_close_fd(fd);
		exit(EXIT_NOTFOUND);
	}
	if (execve(path, cmd, envp) == -1)
	{
		ft_free(cmd);
		ft_close_fd(fd);
		exit(EXIT_FAILURE);
	}
}
