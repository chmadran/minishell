/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 11:50:56 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/29 10:41:49 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

static void	ft_here_sig(int signal)
{
	if (signal == SIGINT)
		exit(1);
}

static void	ft_child_sig(int signal)
{
	if (signal == SIGINT)
		printf("\n");
}

static void	ft_here(char *limiter)
{
	char	*line;
	char	*tmp_filename;
	int		tmp_file_fd;

	tmp_filename = "minishell_heredoc_tmp.txt";
	tmp_file_fd = open(tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_file_fd == -1)
		exit(EXIT_FAILURE);
	while (1)
	{
		line = readline("> ");
		if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		write(tmp_file_fd, line, ft_strlen(line));
		write(tmp_file_fd, "\n", 1);
		free(line);
	}
	close(tmp_file_fd);
	ft_free_child();
	exit(EXIT_SUCCESS);
}

static int	ft_exec_heredoc(t_exec *exec)
{
	char	*limiter;
	int		pid;
	int		position;
	int		redir;

	redir = check_heredoc(exec->argv, &position);
	if (redir == -1)
		return (-1);
	if (exec->argv[redir][position + 2] == '\0')
		limiter = exec->argv[redir + 1];
	else
		limiter = &exec->argv[redir][position + 2];
	pid = fork();
	(signal(SIGQUIT, SIG_IGN), signal(SIGINT, &ft_here_sig));
	if (pid == 0)
		ft_here(limiter);
	waitpid(pid, &g_master.exit_status, 0);
	if (WIFEXITED(g_master.exit_status))
		g_master.exit_status = WEXITSTATUS(g_master.exit_status);
	return (g_master.exit_status);
}

int	launch_heredoc(t_exec *exec)
{
	int	redir;
	int	position;
	int	heredoc_tkn;

	heredoc_tkn = 0;
	redir = check_heredoc(exec->argv, &position);
	while (redir != -1)
	{
		heredoc_tkn = 1;
		signal(SIGQUIT, &ft_child_sig);
		signal(SIGINT, &ft_child_sig);
		ft_exec_heredoc(exec);
		clean_args(exec, redir);
		redir = check_heredoc(exec->argv, &position);
	}
	add_tmp_file(exec, heredoc_tkn);
	return (0);
}
