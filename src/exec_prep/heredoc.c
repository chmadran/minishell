/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmadran <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 11:50:56 by chmadran          #+#    #+#             */
/*   Updated: 2023/08/31 14:25:58 by chmadran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"
#include "utils.h"

void	child_sigint_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		ft_free_child();
		rl_on_new_line();
		g_master.exit_status = 130;
		exit(130);
	}
	if (signal == SIGQUIT)
		ft_putstr_fd("\b\b  \b\b", 2);
	else
	{
		write(1, "\n", 1);
		ft_free_child();
		rl_on_new_line();
		exit(-1);
	}
}

static void	ft_here(char *limiter)
{
	char	*tmp_filename;
	int		tmp_file_fd;

	tmp_filename = "minishell_heredoc_tmp.txt";
	tmp_file_fd = open(tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_file_fd == -1)
		exit(EXIT_FAILURE);
	g_master.heredoc_tmp_file_fd = tmp_file_fd;
	loop_ft_here(tmp_file_fd, limiter);
	fd_close(g_master.heredoc_tmp_file_fd);
	ft_free_child();
	exit(EXIT_SUCCESS);
}

static int	ft_exec_heredoc(t_exec *exec, t_master *master)
{
	char	*limiter;
	int		position;
	int		redir;

	g_master.count_pid = 0;
	redir = check_heredoc(exec->argv, &position);
	if (redir == -1)
		return (-1);
	if (exec->argv[redir][position + 2] == '\0')
		limiter = exec->argv[redir + 1];
	else
		limiter = &exec->argv[redir][position + 2];
	(signal(SIGINT, &child_sigint_heredoc));
	master->pid = fork();
	if (master->pid == 0)
		ft_here(limiter);
	if (master->pid > 0)
		master->child_pid[g_master.count_pid++] = master->pid;
	signal(SIGINT, SIG_IGN);
	wait_all_processes();
	signal(SIGINT, handle_sigint);
	return (g_master.exit_status);
}

int	launch_heredoc(t_exec *exec, t_master *master)
{
	int	redir;
	int	position;
	int	heredoc_tkn;

	heredoc_tkn = 0;
	redir = check_heredoc(exec->argv, &position);
	while (redir != -1)
	{
		heredoc_tkn = 1;
		if (ft_exec_heredoc(exec, master) > 0)
			return (EXIT_FAILURE);
		clean_args(exec, redir);
		redir = check_heredoc(exec->argv, &position);
	}
	add_tmp_file(exec, heredoc_tkn);
	return (EXIT_SUCCESS);
}
