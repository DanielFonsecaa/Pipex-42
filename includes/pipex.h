/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-fons <dda-fons@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:15:42 by dda-fons          #+#    #+#             */
/*   Updated: 2025/06/18 11:57:26 by dda-fons         ###   ########.fr       */
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

typedef struct s_pipeline
{
	int		**pipes;
	pid_t	*pids;
	int		fd[2];
	int		num_cmds;
}	t_pipeline;

// closes and frees
void	ft_close_fd(int *fd);
void	ft_free(char **str);
void	close_fds(int **pipes, int num_pipes, int fd_in, int fd_out);
void	cleanup_pipes(int **pipes, int count);
void	ft_fork_error(t_pipeline *data);

// child and parent process functions
void	ft_child(int *fd, char **argv, char **envp);
void	ft_parent(int *fd, char **argv, char **envp);
void	setup_child(int index, int num_cmds, int **pipes, int *fd);

// utility functions
char	*ft_get_path(char **envp, char *cmd);
void	ft_execute(char *argv, char **envp, int *fd);
int		ft_wait(pid_t pid1, pid_t pid2);
void	ft_process(char **argv, char **envp);
void	cleanup_malloc(int **pipes, int count);
int		wait_and_get_exit_status(pid_t *pids, int num_cmds);

//pipes
int		**create_pipes(int num_pipes);
int		create_single_pipe(int **pipes, int index);
void	init_pipeline(int argc, char **argv, t_pipeline *data, int here_doc);
void	execute_pipeline(char **argv, char **envp, t_pipeline *data, int doc);

// here_doc
void	ft_here(char **argv, int *fd);

#endif
