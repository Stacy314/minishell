///* ************************************************************************** */
///*                                                                            */
///*                                                        :::      ::::::::   */
///*   pipe.c                                             :+:      :+:    :+:   */
///*                                                    +:+ +:+         +:+     */
///*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
///*                                                +#+#+#+#+#+   +#+           */
///*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
///*   Updated: 2025/01/31 17:24:09 by apechkov         ###   ########.fr       */
///*                                                                            */
///* ************************************************************************** */

//#include "../minishell.h"

////exit | exit | exit (shouldn't exit and shouldn't print anything) (exit status 0)
////export GHOST=123 | env | grep GHOST (exit status 1)

////int	handle_redir_out(t_cmd *cmd, t_data *data)  //bool has_child
////{
////	//t_redir_out	*redir;
////	int			res;

////	redir = cmd->redir_struct;
////	while (redir)
////	{
////		if (redir->type == T_R_OUT)
////			res = redir_out(redir, mem, has_child);
////		else if (redir->type == T_OUT_APPEND)
////			res = redir_append(redir, mem, has_child);
////		if (res == 0)
////			return (0);
////		redir = redir->next;
////	}
////	return (1);
////}

////int	handle_redir_in(t_cmd *cmd, t_data *data)
////{
////	//t_redir_out	*redir;
////	int			res;

////	redir = cmd->redir_struct;
////	while (redir)
////	{
////		if (redir->type == T_R_IN)
////			res = redir_in(cmd, redir, mem, has_child);
////		else if (redir->type == T_HEREDOC)
////			res = redir_heredoc(cmd, redir, mem, has_child);
////		if (res == 0)
////			return (0);
////		redir = redir->next;
////	}
////	return (1);
////}

//pid_t	execute_first_command(t_cmd **cmd, t_data **data,char **env, int pipe_fd[2])
//{
//	(void)data;
//	//dup2(pipe_fd[1], STDOUT_FILENO);
//    //close(pipe_fd[0]);
//    //close(pipe_fd[1]);
//	  if (pipe(pipe_fd) == -1)
//    {
//        perror("pipe");
//        return (-1);
//    }
//    pid_t pid = fork();
//    if (pid < 0)
//    {
//        perror("fork");
//        return (-1);
//    }

//    if (pid == 0)
//    {
//        // CHILD
//        dup2(pipe_fd[1], STDOUT_FILENO);
//        close(pipe_fd[0]);
//        close(pipe_fd[1]);
//        // Example: if cmd->args = ["ls", "-l", NULL],
//        // you might need the full path or do a PATH search.
//        execve((*cmd)->args[0], (*cmd)->args, env);
//        //perror("execve");
//        exit(127);
//    }
//    else
//    {
//        close(pipe_fd[1]);
//    }
//    return (pid);
//}
//pid_t	execute_middle_command(t_cmd **cmd, t_data **data,char **env, int old_pipe_fd[2], int new_pipe_fd[2])
//{
//	(void)data;
//    if (pipe(new_pipe_fd) == -1)
//    {
//        perror("pipe");
//        return (-1);
//    }
//    pid_t pid = fork();
//    if (pid < 0)
//    {
//        perror("fork");
//        return (-1);
//    }

//    if (pid == 0)
//    {
//        // CHILD
//        dup2(old_pipe_fd[0], STDIN_FILENO);
//        dup2(new_pipe_fd[1], STDOUT_FILENO);
//        close(old_pipe_fd[0]);
//        close(old_pipe_fd[1]);
//        close(new_pipe_fd[0]);
//        close(new_pipe_fd[1]);
//        execve((*cmd)->args[0], (*cmd)->args, env);
//        //perror("execve");
//        exit(127);
//    }
//    else
//    {
//        // PARENT
//        close(old_pipe_fd[0]);
//        close(old_pipe_fd[1]);
//        close(new_pipe_fd[1]);
//    }
//    return (pid);
//}

// pid_t	execute_last_command(t_cmd **cmd, t_data **data,char **env, int pipe_fd[2])
// {
//	(void)data;
//    pid_t pid = fork();
//    if (pid < 0)
//    {
//        perror("fork");
//        return (-1);
//    }

//    if (pid == 0)
//    {
//        // CHILD
//        dup2(pipe_fd[0], STDIN_FILENO);
//        close(pipe_fd[0]);
//        close(pipe_fd[1]);
//        execve((*cmd)->args[0], (*cmd)->args, env);
       /// perror("execve");
//        exit(127);
//    }
//    else
//    {
//        // PARENT
//        close(pipe_fd[0]);
//        close(pipe_fd[1]);
//    }
//    return (pid);
// }

////void execute_pipeline(t_cmd **cmd, t_data *data, t_token *tokens, char **env)
////{
////	(void)env;
////	(void)tokens;
////    int pipe_fd[2];
////	//int pipe_fd2[2];
////    int i = 0;
////    //pid_t *pid;
////    int status;

////    if (!cmd || !cmd[0] || !cmd[0]->args)
////    {
////        fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
////        data->exit_status = 1;
////        return;
////    }
////	if (cmd[i + 1])
////	{
////		execute_first_command(cmd, &data, env, pipe_fd);
////	}
////    cmd = cmd->next;
////	while (cmd->next)
////	{
////		execute_middle_command(cmd, &data, env, pipe_fd);
////		cmd = cmd->next;
////	}
////	execute_last_command(cmd, &data, env, pipe_fd);
////	close(pipe_fd[0]);
////	close(pipe_fd[1]);
////	//handle_multiprocess(data, i);
////}

//int count_commands(t_cmd *cmd)
//{
//    int count = 0;

//    while (cmd[count].args != NULL)
//    {
//        count++;
//    }
//    return count;
//}

//void execute_pipeline(t_cmd **cmd, t_data *data, char **env)
//{

//    int n_cmds = count_commands(*cmd);
//    if (n_cmds == 0)
//        return;
    
//    pid_t pids[n_cmds];
//    int old_pipe_fd[2]; 
//    int new_pipe_fd[2]; 

//    t_cmd *current_cmd = cmd[0];
//    pids[0] = execute_first_command(cmd, &data, env, old_pipe_fd);

//    int i = 1;
//    for (; i < n_cmds - 1; i++)
//    {
//        current_cmd = cmd[i];
//        pids[i] = execute_middle_command(&current_cmd, &data, env, old_pipe_fd, new_pipe_fd);
//        old_pipe_fd[0] = new_pipe_fd[0];
//        old_pipe_fd[1] = new_pipe_fd[1];
//    }
	
//    current_cmd = cmd[i];
//    pids[i] = execute_last_command(&current_cmd, &data, env, old_pipe_fd);


//    int status;
//    for (i = 0; i < n_cmds; i++)
//    {
//        waitpid(pids[i], &status, 0);
//        if (WIFEXITED(status))
//            data->exit_status = WEXITSTATUS(status);
//    }
//}

#include "../minishell.h"

char *find_command_path(const char *cmd, char **env)
{
    int i = 0;
    char *path_env = NULL;
    char *token = NULL;
    char *full_path = NULL;
    size_t len_dir, len_cmd;
    
    // Find the PATH variable in env
    while (env[i])
    {
        if (strncmp(env[i], "PATH=", 5) == 0)
        {
            path_env = env[i] + 5; // Skip "PATH="
            break;
        }
        i++;
    }
    
    if (!path_env)
    {
        // If no PATH is found, we can't search.
        return NULL;
    }
    
    // Duplicate the PATH variable so strtok doesn't modify the original.
    char *paths = strdup(path_env);
    if (!paths)
    {
        perror("strdup");
        return NULL;
    }
    
    len_cmd = strlen(cmd);
    token = strtok(paths, ":");
    while (token)
    {
        len_dir = strlen(token);
        // Allocate memory for full path: directory + "/" + command + '\0'
        full_path = malloc(len_dir + 1 + len_cmd + 1);
        if (!full_path)
        {
            perror("malloc");
            free(paths);
            return NULL;
        }
        // Build the full path string
        strcpy(full_path, token);
        strcat(full_path, "/");
        strcat(full_path, cmd);
        
        // Check if the file exists and is executable.
        if (access(full_path, X_OK) == 0)
        {
            free(paths);
            return full_path; // Caller must free this string later.
        }
        
        free(full_path);
        full_path = NULL;
        
        // Get the next directory.
        token = strtok(NULL, ":");
    }
    
    free(paths);
    return NULL; // Command not found in any PATH directory.
}
int count_commands(t_cmd *cmd)
{
    int count = 0;

    while (cmd[count].args != NULL)
    {
        count++;
    }
    return count;
}

pid_t execute_first_command(t_cmd **cmd, t_data **data, char **env, int pipe_fd[2])
{
	(void)data;
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return (-1);
    }

    if (pid == 0) // CHILD
    {
        dup2(pipe_fd[1], STDOUT_FILENO); // Redirect output to the pipe
        close(pipe_fd[0]);
        close(pipe_fd[1]);

		//char *cmd_path = NULL;
        //if (strchr((*cmd)->args[0], '/'))
        //    cmd_path = (*cmd)->args[0];
        //else
        //    cmd_path = find_command_path((*cmd)->args[0], env);
        
        //if (!cmd_path)
        //{
        //    fprintf(stderr, "minishell: %s: command not found\n", (*cmd)->args[0]);
        //    exit(127);
        //}

        execve((*cmd)->args[0], (*cmd)->args, env);
        perror("execve"); // Error in case execve fails
        exit(127);
    }
    else // PARENT
    {
        close(pipe_fd[1]); // Close the write end of the pipe
    }
    return (pid);
}

pid_t execute_middle_command(t_cmd **cmd, t_data **data, char **env, int old_pipe_fd[2], int new_pipe_fd[2])
{
	(void)data;
    if (pipe(new_pipe_fd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return (-1);
    }

    if (pid == 0) // CHILD
    {
        dup2(old_pipe_fd[0], STDIN_FILENO); // Read from the old pipe
        dup2(new_pipe_fd[1], STDOUT_FILENO); // Write to the new pipe
        close(old_pipe_fd[0]);
        close(old_pipe_fd[1]);
        close(new_pipe_fd[0]);
        close(new_pipe_fd[1]);
        execve((*cmd)->args[0], (*cmd)->args, env);
        //perror("execve");
        exit(127);
    }
    else // PARENT
    {
        close(old_pipe_fd[0]); // Close the old pipe in the parent
        close(old_pipe_fd[1]);
        close(new_pipe_fd[1]); // Close the write end of the new pipe
    }
    return (pid);
}

pid_t execute_last_command(t_cmd **cmd, t_data **data, char **env, int pipe_fd[2])
{
	(void)data;
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return (-1);
    }

    if (pid == 0) // CHILD
    {
        dup2(pipe_fd[0], STDIN_FILENO); // Read from the pipe
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        
        // Special handling for echo | echo
        if ((*cmd)->args[0] && strcmp((*cmd)->args[0], "echo") == 0)
        {
            char buffer[1024];
            ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0)
            {
                buffer[bytes_read] = '\0'; // Null-terminate the buffer
                write(STDOUT_FILENO, buffer, bytes_read); // Write the content
            }
            else
            {
                write(STDOUT_FILENO, "\n", 1); // Just print a new line
            }
            exit(0); // Exit after handling echo
        }

        execve((*cmd)->args[0], (*cmd)->args, env);
        //perror("execve");
        exit(127);
    }
    else // PARENT
    {
        close(pipe_fd[0]); // Close the read end of the pipe in the parent
        close(pipe_fd[1]);
    }
    return (pid);
}

void execute_pipeline(t_cmd **cmd, t_data *data, char **env)
{
    int n_cmds = count_commands(*cmd);
    if (n_cmds == 0)
        return;

    pid_t pids[n_cmds];
    int old_pipe_fd[2];
    int new_pipe_fd[2];

    t_cmd *current_cmd = cmd[0];
    pids[0] = execute_first_command(cmd, &data, env, old_pipe_fd);

    int i = 1;
    for (; i < n_cmds - 1; i++)
    {
        current_cmd = cmd[i];
        pids[i] = execute_middle_command(&current_cmd, &data, env, old_pipe_fd, new_pipe_fd);
        old_pipe_fd[0] = new_pipe_fd[0];
        old_pipe_fd[1] = new_pipe_fd[1];
    }

    current_cmd = cmd[i];
    pids[i] = execute_last_command(&current_cmd, &data, env, old_pipe_fd);

    int status;
    for (i = 0; i < n_cmds; i++)
    {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status))
            data->exit_status = WEXITSTATUS(status);
    }
}







/////////////////////////////////////////////////////////////////////////////

//pid_t execute_last_command(t_cmd **cmd, t_data **data, char **env, int pipe_fd[2])
//{
//    (void)data;
//    pid_t pid = fork();
//    if (pid < 0)
//    {
//        perror("fork");
//        return (-1);
//    }

//    if (pid == 0) // CHILD process
//    {
//        dup2(pipe_fd[0], STDIN_FILENO); // Встановлюємо вхідні дані з пайпа
//        close(pipe_fd[0]);
//        close(pipe_fd[1]);

//        execve((*cmd)->args[0], (*cmd)->args, env);
//        perror("execve"); // Якщо execve не вдасться, виводимо помилку
//        exit(127);
//    }
//    else
//    {
//        // PARENT
//        close(pipe_fd[0]);
//        close(pipe_fd[1]);
//    }
//    return (pid);
//}



//void execute_pipeline(t_cmd **cmd, t_data *data, char **env)
//{
//    int n_cmds = count_commands(*cmd);
//    if (n_cmds == 0)
//        return;
    
//    pid_t pids[n_cmds];
//    int old_pipe_fd[2]; 
//    int new_pipe_fd[2]; 

//    t_cmd *current_cmd = cmd[0];
//    pids[0] = execute_first_command(cmd, &data, env, old_pipe_fd); // Перша команда

//    int i = 1;
//    for (; i < n_cmds - 1; i++)
//    {
//        current_cmd = cmd[i];
//        pids[i] = execute_middle_command(&current_cmd, &data, env, old_pipe_fd, new_pipe_fd); // Середні команди
//        old_pipe_fd[0] = new_pipe_fd[0];
//        old_pipe_fd[1] = new_pipe_fd[1];
//    }
    
//    current_cmd = cmd[i];
//    pids[i] = execute_last_command(&current_cmd, &data, env, old_pipe_fd); // Остання команда

//    int status;
//    for (i = 0; i < n_cmds; i++)
//    {
//        waitpid(pids[i], &status, 0);
//        if (WIFEXITED(status))
//            data->exit_status = WEXITSTATUS(status);
//    }
//}
