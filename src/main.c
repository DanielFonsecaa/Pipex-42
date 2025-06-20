/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-fons <dda-fons@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:12:53 by dda-fons          #+#    #+#             */
/*   Updated: 2025/06/10 19:04:53 by dda-fons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	ft_child(int *fd, char **argv, char **envp)
{
	int	new_fd;

	close(fd[0]);
	new_fd = open(argv[1], O_RDONLY);
	if (new_fd == -1)
	{
		perror(argv[1]);
		ft_close_fd(fd);
		exit(EXIT_FAILURE);
	}
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	dup2(new_fd, STDIN_FILENO);
	close(new_fd);
	ft_execute(argv[2], envp, fd);
}

void	ft_parent(int *fd, char **argv, char **envp)
{
	int	new_fd;

	close(fd[1]);
	new_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (new_fd == -1)
	{
		perror(argv[4]);
		ft_close_fd(fd);
		exit(EXIT_FAILURE);
	}
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	dup2(new_fd, STDOUT_FILENO);
	close(new_fd);
	ft_execute(argv[3], envp, fd);
}

int	ft_wait(pid_t pid1, pid_t pid2)
{
	int	status1;
	int	status2;

	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	else
		return (EXIT_FAILURE);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
		exit(EXIT_FAILURE);
	}
	if (pipe(fd) == -1)
		perror("pipe");
	pid1 = fork();
	if (pid1 == -1)
		perror("fork");
	else if (pid1 == 0)
		ft_child(fd, argv, envp);
	pid2 = fork();
	if (pid2 == -1)
		perror("fork");
	else if (pid2 == 0)
		ft_parent(fd, argv, envp);
	ft_close_fd(fd);
	return (ft_wait(pid1, pid2));
}
