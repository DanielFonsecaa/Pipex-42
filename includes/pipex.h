/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-fons <dda-fons@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:15:42 by dda-fons          #+#    #+#             */
/*   Updated: 2025/06/11 15:06:39 by dda-fons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

//macros for exit codes
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define EXIT_MISUSE 2
# define EXIT_NOTEXEC 126
# define EXIT_NOTFOUND 127
# define EXIT_INVALID 128

# include "../lib/libft/libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>

// closes and frees
void	ft_close_fd(int *fd);
void	ft_free(char **str);

// child and parent process functions
void	ft_child(int *fd, char **argv, char **envp);
void	ft_parent(int *fd, char **argv, char **envp);

// utility functions
char	*ft_get_path(char **envp, char *cmd);
void	ft_execute(char *argv, char **envp, int *fd);
int		ft_wait(pid_t pid1, pid_t pid2);
void	ft_process(char **argv, char **envp);

#endif
