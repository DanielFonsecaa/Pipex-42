/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-fons <dda-fons@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:37:41 by dda-fons          #+#    #+#             */
/*   Updated: 2025/06/16 16:27:50 by dda-fons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/pipex.h"

void    ft_here(char **argv, int *fd)
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
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0 &&
			(line[ft_strlen(limiter)] == '\n' ||
			line[ft_strlen(limiter) + 1] == '\0'))
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
