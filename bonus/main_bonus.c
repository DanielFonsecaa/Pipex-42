/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-fons <dda-fons@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 13:13:37 by dda-fons          #+#    #+#             */
/*   Updated: 2025/06/20 11:08:55 by dda-fons         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipeline	data;
	int			last_exit_status;
	int			here_doc;

	here_doc = (argc > 5 && ft_strcmp(argv[1], "here_doc") == 0);
	if (argc >= 5)
	{
		if (here_doc)
			data.num_cmds = argc - 4;
		else
			data.num_cmds = argc - 3;
		init_pipeline(argc, argv, &data, here_doc);
		execute_pipeline(argv, envp, &data, here_doc);
		close_fds(data.pipes, data.num_cmds - 1, data.fd[0], data.fd[1]);
		last_exit_status = wait_and_get_exit_status(data.pids, data.num_cmds);
		cleanup_pipes(data.pipes, data.num_cmds - 1, &data);
		if (data.fd[1] == -1)
			exit(EXIT_FAILURE);
		exit(last_exit_status);
	}
	ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 cmd3 cmdn file2\n", 2);
	exit(EXIT_FAILURE);
}
