/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-fons <dda-fons@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:12:53 by dda-fons          #+#    #+#             */
/*   Updated: 2025/06/13 12:52:48 by dda-fons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	ft_wait(pid_t pid1, pid_t pid2);
void	ft_process(char **argv, char **envp);

int	main(int argc, char **argv, char **envp)
{
	int		fd_in;
	int		fd_out;
	int		i;

	if (argc >= 5)
	{
		i = 2;
		fd_in = open(argv[1], O_RDONLY);
		if (fd_in == -1)
		{
            perror(argv[1]);
            exit(EXIT_FAILURE);
        }
		else
		{
			dup2(fd_in, STDIN_FILENO);
			close(fd_in);
		}
		fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd_out == -1)
		{
			perror(argv[argc - 1]);
			exit(EXIT_FAILURE);
		}
		while (i < argc - 2)
			ft_process(&argv[i++], envp);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
		ft_execute(argv[argc - 2], envp, NULL);
	    exit(EXIT_SUCCESS);
	}
	ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 cmd3 cmdn file2\n", 2);
	exit(EXIT_FAILURE);
}

void	ft_process(char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
	{
        perror("pipe");
        exit(EXIT_FAILURE);
    }
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		ft_close_fd(fd);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		ft_execute(*argv, envp, fd);
		exit(EXIT_FAILURE);
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	waitpid(pid, NULL, 0);
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
