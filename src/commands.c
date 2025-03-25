/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallyam <mgallyam@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/25 17:28:47 by mgallyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// ls | wc -l
// unset path

//"" (: command not found, EC - 127) (11)
// touch "" (touch: cannot touch '': No such file or directory) (13)

// awk 'BEGIN{for(i=1;i<=10;i++){for(j=1;j<=10;j++){printf("%4d ",i*j)} printf("\n")}}'
/// dev/null | tail -n 10 (36)
// awk 'BEGIN{srand(42); for(i=1;i<=1000000;i++)print int(rand()*1000)}' | awk '{sum+=$1} END {print sum/NR}' (38)

//"."

// unset PATH
// change to /bin/ - inside this directory the standard commands should work

static struct sigaction g_old_int;
static struct sigaction g_old_quit;

// Функция, которая заставит parent (minishell) игнорировать SIGINT/SIGQUIT
void parent_ignore_signals(void)
{
    struct sigaction sa_ignore;

    sa_ignore.sa_handler = SIG_IGN;
    sa_ignore.sa_flags = 0;
    sigemptyset(&sa_ignore.sa_mask);

    // Считываем старые обработчики, чтобы потом вернуть
    sigaction(SIGINT, NULL, &g_old_int);
    sigaction(SIGQUIT, NULL, &g_old_quit);

    // Ставим SIG_IGN
    sigaction(SIGINT, &sa_ignore, NULL);
    sigaction(SIGQUIT, &sa_ignore, NULL);
}

// Восстановим родительские обработчики сигналов
void parent_restore_signals(void)
{
    sigaction(SIGINT, &g_old_int, NULL);
    sigaction(SIGQUIT, &g_old_quit, NULL);
}

int	check_permissions(char *cmd)
{
	struct stat	buf;

	if (stat(cmd, &buf) == -1)
	{
		write_error("minishell: %s: No such file or directory\n", cmd);
		return (127); // 2
	}
	if (S_ISDIR(buf.st_mode))
	{
		write_error("minishell: %s: Is a directory\n", cmd);
		return (126);
	}
	if (access(cmd, X_OK) == -1)
	{
		write_error("minishell: %s: Permission denied\n", cmd);
		return (126);
	}
	return (0);
}

// static int	fork_and_exec(const char *executable, char **args, char **env,
// 		t_data *data)
// {
// 	pid_t	pid;
// 	int		status;

// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("fork");
// 		return (data->exit_status = 1);
// 	}
// 	//signal(SIGINT, SIG_IGN);
// 	if (pid == 0)
// 	{


// 		set_signals_child(); //
// 		//signal(SIGINT, SIG_DFL);
// 		execve(executable, args, env);
// 		// perror("execve");
// 		exit(0); // 127 ?
// 	}
// 	else
// 	{
// 		waitpid(pid, &status, 0);
// 		if (WIFEXITED(status))
// 		{
// 			///* Katya */
// 			//if (WEXITSTATUS(status) == 130 || WEXITSTATUS(status) == 131)
// 			//{
// 			//	if (WEXITSTATUS(status) == 131)
// 			//		printf("Quit (core dumped)");
// 			//	write (1, "\n", 1);
// 			//}
// 			///* End */
// 			data->exit_status = WEXITSTATUS(status);
// 		}
// 		else
// 			data->exit_status = 1;
// 	}
// 	//printf("\n"); // wrong
// 	//signal(SIGINT, handle_sigint);
// 	return (data->exit_status);
// }

static int fork_and_exec(const char *executable, char **args, char **env, t_data *data)
{
    pid_t pid;
    int status;

	parent_ignore_signals();

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
		parent_restore_signals();
		data->exit_status = 1;
        return (data->exit_status);
    }
    else if (pid == 0)
    {
        // -- Дочерний процесс --
        // ВОССТАНАВЛИВАЕМ СИГНАЛЫ К УМОЛЧАНИЮ
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);

        // Запускаем внешнюю программу
        execve(executable, args, env);

        // Если execve вернулся — значит была ошибка
        perror("execve");
        _exit(127); // bash-совместимый код ошибки
    }
    else
    {
        // -- Родительский процесс (minishell) --
        waitpid(pid, &status, 0);
		parent_restore_signals();

		//fprintf(stderr, "[DEBUG] after waitpid, status = %d\n", status);

        if (WIFSIGNALED(status))
        {
            // Процесс убит сигналом
            int sig = WTERMSIG(status);
            if (sig == SIGINT)
            {
                // Ctrl-C убил дочерний процесс
                // Bash обычно при этом выводит просто \n
                // и exit-код = 130
                data->exit_status = 130;
                write(STDOUT_FILENO, "\n", 1);
            }
            else if (sig == SIGQUIT)
            {
                // Ctrl-\ убил дочерний процесс
                // Bash выводит "Quit (core dumped)\n" и exit-код = 131
                data->exit_status = 131;
                write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
            }
            else
            {
                // Можно сделать общий случай 128 + sig
                data->exit_status = 128 + sig;
            }
        }
        else if (WIFEXITED(status))
        {
            // Процесс завершился обычным вызовом exit(...)
            data->exit_status = WEXITSTATUS(status);
        }
        else
        {
            // На всякий случай, если какая-то другая причина
            data->exit_status = 1;
        }
    }
    return data->exit_status;
}


static int	execute_direct_path(char *cmd, t_data *data, char **args,
		char **env)
{
	int	error_code;

	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		error_code = check_permissions(cmd);
		if (error_code)
			return (error_code);
		return (fork_and_exec(cmd, args, env, data));
	}
	return (-1);
}

static int	execute_via_path(char *cmd, t_data *data, char **args, char **env)
{
	char	*path;
	char	**paths;
	char	*executable;
	int		i;

	path = get_path_from_env(env);
	if (!path)
		return (write_error("%s: command not found\n", cmd),
			data->exit_status = 127);
	paths = split_path(path);
	if (!paths)
	{
		return (data->exit_status = 1);
	}
	executable = find_executable(cmd, paths);
	if (!executable)
	{
		write_error("%s: command not found\n", cmd);
		return (free_array(paths), data->exit_status = 127);
	}
	(fork_and_exec(executable, args, env, data), free(executable));
	i = 0;
	while (paths[i])
		free(paths[i++]);
	return (free(paths), data->exit_status);
}

int	execute_command(char *cmd, t_data *data, char **args, char **env)
{
	int	result;

	result = execute_direct_path(cmd, data, args, env);
	if (result >= 0)
		return (result);
	return (execute_via_path(cmd, data, args, env));
}
