
#include "minishell.h"
#include "libft.h"
#include "exit.h"
#include "env.h"
#include "exec.h"



int	check_heredoc(char **argv, int *position)
{
	int	i;
	int	j;

	i = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (ft_strncmp(&argv[i][j], "<<", 2) == 0)
			{
				*position = j;
				return (i);
			}
			j++;
		}
		i++;
	}
	return (-1);
}


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

static void ft_here(char *limiter) {
    char *line;
    char *tmp_filename = "minishell_heredoc_tmp.txt";
    int tmp_file_fd = open(tmp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (tmp_file_fd == -1) {
        perror("Error opening tmp file");
        exit(EXIT_FAILURE);
    }

    while (1) {
        signal(SIGQUIT, SIG_IGN);
        signal(SIGINT, &ft_here_sig);
        line = readline("> ");

        if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0) {
            free(line);
            break;
        }

        write(tmp_file_fd, line, ft_strlen(line));
        write(tmp_file_fd, "\n", 1);
        
        free(line);
    }
    
    close(tmp_file_fd);
}




static int	ft_exec_heredoc(t_exec *exec)
{
	char	*limiter;
	int		pid;
	int		position;
	int     redir;

	redir = check_heredoc(exec->argv, &position);
	if (redir == -1)
		return (-1);

	if (exec->argv[redir][position + 2] == '\0')
		limiter = exec->argv[redir + 1];
	else
		limiter = &exec->argv[redir][position + 2];
		

	pid = fork();
	if (pid == 0)
		ft_here(limiter);
	waitpid(pid, &g_master.exit_status, 0);
	if (WIFEXITED(g_master.exit_status))
		g_master.exit_status = WEXITSTATUS(g_master.exit_status);
	return (g_master.exit_status);
}



void clean_args(t_exec *exec, int heredoc_index)
{
    int i;
		
		i = heredoc_index;

    // Libération de la mémoire allouée pour '<<' et le limiter.
    free(exec->argv[heredoc_index]);  // '<<'
    free(exec->argv[heredoc_index + 1]);  // limiter

    // Décalage des pointeurs pour supprimer '<<' et le limiter.
    while (exec->argv[i + 2])
    {
        exec->argv[i] = exec->argv[i + 2];
        i++;
    }
    exec->argv[i] = NULL;  // Mettre le dernier élément à NULL.

    // Ajustement de argc.
    exec->argc -= 2;
}

void add_and_swap_args(t_exec *exec, int htkn)
{
		int	i;

		i = 0;
    if (ft_strcmp(exec->argv[0], "cat") == 0 && htkn == 1 && !exec->argv[1])
		{
			exec->argc++; 
			exec->argv[1] = ft_strdup("minishell_heredoc_tmp.txt");
			exec->argv[2] = NULL;


		}
    if (ft_strcmp(exec->argv[0], "cat") == 0 && ft_strcmp(exec->argv[1], ">") == 0 && htkn == 1)
    {
        // Augmenter la taille de argv
        char **new_argv = (char **)malloc(sizeof(char *) * (exec->argc + 2)); // +2 pour le nouvel élément et NULL à la fin
        if (!new_argv)
        {
            perror("Error allocating memory for new argv");
            exit(EXIT_FAILURE);
        }

        // Ajouter "minishell_heredoc_tmp.txt" en premier
        new_argv[0] = ft_strdup("minishell_heredoc_tmp.txt");

        // Copier les anciens éléments
				while (i < exec->argc)
				{
    			new_argv[i + 1] = exec->argv[i];
    			i++;
				}
        new_argv[exec->argc + 1] = NULL;  // Terminer par NULL


        free(exec->argv);  // Libérer l'ancien argv
        exec->argv = new_argv;  // Affecter le nouveau argv
        exec->argc++;  // Incrémenter argc

        // Maintenant, échanger "cat" et "minishell_heredoc_tmp.txt"
        char *temp = exec->argv[0];
        exec->argv[0] = exec->argv[1];
        exec->argv[1] = temp;
    }
}

int	launch_heredoc(t_exec *exec)
{
	int	redir;
	int position;
	int htkn;

	htkn = 0;
	redir = check_heredoc(exec->argv, &position);
	while (redir != -1)
	{
		htkn = 1;
		signal(SIGQUIT, &ft_child_sig);
		signal(SIGINT, &ft_child_sig);
		ft_exec_heredoc(exec);
		clean_args(exec, redir);
		redir = check_heredoc(exec->argv, &position);

	}
	add_and_swap_args(exec, htkn);
	return (0);
}