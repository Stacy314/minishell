///* ************************************************************************** */
///*                                                                            */
///*                                                        :::      ::::::::   */
///*   redirection.c                                      :+:      :+:    :+:   */
///*                                                    +:+ +:+         +:+     */
///*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
///*                                                +#+#+#+#+#+   +#+           */
///*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
///*   Updated: 2025/04/02 17:17:29 by apechkov         ###   ########.fr       */
///*                                                                            */
///* ************************************************************************** */

//#include "../../minishell.h"

//// Ambigious redirection
//// export a="a b"
//// > $a (bash: $a: ambiguous redirect)

//// export a="a b"
//// echo > $a | echo > $a | echo > $a | echo hi

//// > $notexists
//// bash: $notexists: ambiguous redirect

//// echo <"./test_files/infile_big" | cat <"./test_files/infile" (18)
//// cat <missing | cat <"./test_files/infile" (38)

//// cat <minishell.h|ls (118) - parsing

//static void	redir_loop(t_data *data, const char *input)
//{
//	int	i;

//	i = 0;
//	while (input[i])
//	{
//		if (input[i] == '>' && input[i + 1] == '>')
//		{
//			handle_append_redirect(data);
//			i += 2;
//			break ;
//		}
//		else if (input[i] == '<' && input[i + 1] == '<')
//		{
//			execute_heredoc(data->cmd);
//			i += 2;
//			break ;
//		}
//		else if (input[i] == '<')
//		{
//			handle_input_redirect(data);
//			i++;
//			break ;
//		}
//		else if (input[i] == '>')
//		{
//			handle_output_redirect(data);
//			i++;
//			break ;
//		}
//		i++;
//	}
//}

//int	execute_redirection(t_cmd *cmd, t_data *data, t_token **tokens)
//{
//	pid_t	pid;
//	int		status;
//	int		sig;

//	parent_ignore_signals();
//	pid = fork();
//	if (pid == -1)
//		return (perror("fork"), 0);
//	if (pid == 0)
//	{
//		data->is_child = true;
//		signal(SIGINT, SIG_DFL);
//		signal(SIGQUIT, SIG_IGN);
//		redir_loop(data, data->input);
//		signal(SIGQUIT, SIG_DFL);
//		(execute_for_one(tokens, cmd, data),/* close(STDIN_FILENO), close(STDOUT_FILENO),*/ free_all(data, tokens, cmd), exit(data->exit_status));
//	}
//	waitpid(pid, &status, 0);
//	if (WIFEXITED(status))
//	{
//		parent_restore_signals(); // need to move to signals
//		if (WIFSIGNALED(status))
//		{
//			sig = WTERMSIG(status);
//			if (sig == SIGINT)
//			{
//				write(STDOUT_FILENO, "\n", 1);
//				data->exit_status = 130;
//			}
//			else if (sig == SIGQUIT)
//			{
//				write(STDERR_FILENO, "Quit (core dumped)\n", 19);
//				data->exit_status = 131;
//			}
//			else
//				data->exit_status = 128 + sig;
//		}
//		else if (WIFEXITED(status))
//			data->exit_status = WEXITSTATUS(status);
//	}
//	return (1);
//}

//void	apply_redirections(t_cmd *cmd, t_data *data)
//{
//	int	i;

//	if (cmd->heredoc_delimiter && cmd->heredoc_fd != -1)
//	{
//		if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
//		{
//			perror("dup2 heredoc");
//			exit(1);
//		}
//		close(cmd->heredoc_fd);
//	}
//	i = 0;
//	while (data->input[i])
//	{
//		if (data->input[i] == '>' && data->input[i + 1] == '>')
//			handle_append_redirect(data);
//		else if (data->input[i] == '<' && data->input[i + 1] == '<')
//			;
//		else if (data->input[i] == '<')
//			handle_input_redirect(data);
//		else if (data->input[i] == '>')
//			handle_output_redirect(data);
//		i++;
//	}
//}


////// Helper function to extract filename (implement based on your parsing logic)
////char *extract_filename(char *input, int start)
////{
////	// Skip whitespace
////	while (input[start] && (input[start] == ' ' || input[start] == '\t'))
////		start++;
	
////	// Extract the filename until next whitespace or redirection operator
////	int end = start;
////	while (input[end] && input[end] != ' ' && input[end] != '\t' && 
////		   input[end] != '<' && input[end] != '>')
////		end++;
	
////	// Allocate and copy the filename
////	char *filename = malloc(end - start + 1);
////	if (!filename)
////		return NULL;
	
////	int j = 0;
////	for (int i = start; i < end; i++)
////		filename[j++] = input[i];
////	filename[j] = '\0';
	
////	return filename;
////}

////// New function to process redirections during parsing (call this before execution)
////// This should replace your current approach of parsing redirections during execution
////t_redirect *process_redirections(char *input)
////{
////    t_redirect *head = NULL;
////    t_redirect *current = NULL;
////    int i = 0;
    
////    while (input[i])
////    {
////        if (input[i] == '>' && input[i + 1] == '>')
////        {
////            // Handle append redirection (>>)
////            t_redirect *new = malloc(sizeof(t_redirect));
////            if (!new)
////                return (head); // Error handling
                
////            new->type = REDIRECT_APPEND;
////            // Extract filename (this is simplified, you'll need proper parsing)
////            new->filename = extract_filename(input, i + 2);
////            new->fd = -1;
////            new->next = NULL;
            
////            // Add to linked list
////            if (!head)
////                head = new;
////            else
////                current->next = new;
////            current = new;
            
////            i += 2; // Skip >>
////        }
////        else if (input[i] == '<' && input[i + 1] == '<')
////        {
////            // Handle heredoc (<<)
////            t_redirect *new = malloc(sizeof(t_redirect));
////            if (!new)
////                return (head); // Error handling
                
////            new->type = REDIRECT_HEREDOC;
////            // Extract delimiter
////            new->filename = extract_filename(input, i + 2);
////            new->fd = -1;
////            new->next = NULL;
            
////            // Add to linked list
////            if (!head)
////                head = new;
////            else
////                current->next = new;
////            current = new;
            
////            i += 2; // Skip <<
////        }
////        else if (input[i] == '<')
////        {
////            // Handle input redirection (<)
////            t_redirect *new = malloc(sizeof(t_redirect));
////            if (!new)
////                return (head); // Error handling
                
////            new->type = REDIRECT_INPUT;
////            // Extract filename
////            new->filename = extract_filename(input, i + 1);
////            new->fd = -1;
////            new->next = NULL;
            
////            // Add to linked list
////            if (!head)
////                head = new;
////            else
////                current->next = new;
////            current = new;
            
////            i += 1; // Skip <
////        }
////        else if (input[i] == '>')
////        {
////            // Handle output redirection (>)
////            t_redirect *new = malloc(sizeof(t_redirect));
////            if (!new)
////                return (head); // Error handling
                
////            new->type = REDIRECT_OUTPUT;
////            // Extract filename
////            new->filename = extract_filename(input, i + 1);
////            new->fd = -1;
////            new->next = NULL;
            
////            // Add to linked list
////            if (!head)
////                head = new;
////            else
////                current->next = new;
////            current = new;
            
////            i += 1; // Skip >
////        }
////        else
////            i++;
////    }
    
////    return head;
////}

////// Updated apply_redirections function
////int apply_redirections(t_cmd *cmd, t_data *data)
////{
////	(void) data; // Unused parameter, remove if not needed
////    // Process heredoc first if present
////    if (cmd->heredoc_delimiter && cmd->heredoc_fd != -1)
////    {
////        if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
////        {
////            perror("dup2 heredoc");
////            return 0; // Error
////        }
////        close(cmd->heredoc_fd);
////        cmd->heredoc_fd = -1; // Mark as closed
////    }
    
////    // Process the redirections stored in the command structure
////    t_redirect *redirect = cmd->redirections; // You'll need to add this field to t_cmd
    
////    while (redirect)
////    {
////        int fd;
        
////        switch (redirect->type)
////        {
////            case REDIRECT_INPUT:
////                fd = open(redirect->filename, O_RDONLY);
////                if (fd == -1)
////                {
////                    perror(redirect->filename);
////                    return 0; // Error
////                }
////                if (dup2(fd, STDIN_FILENO) == -1)
////                {
////                    perror("dup2");
////                    close(fd);
////                    return 0; // Error
////                }
////                close(fd);
////                break;
                
////            case REDIRECT_OUTPUT:
////                fd = open(redirect->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
////                if (fd == -1)
////                {
////                    perror(redirect->filename);
////                    return 0; // Error
////                }
////                if (dup2(fd, STDOUT_FILENO) == -1)
////                {
////                    perror("dup2");
////                    close(fd);
////                    return 0; // Error
////                }
////                close(fd);
////                break;
                
////            case REDIRECT_APPEND:
////                fd = open(redirect->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
////                if (fd == -1)
////                {
////                    perror(redirect->filename);
////                    return 0; // Error
////                }
////                if (dup2(fd, STDOUT_FILENO) == -1)
////                {
////                    perror("dup2");
////                    close(fd);
////                    return 0; // Error
////                }
////                close(fd);
////                break;
                
////            case REDIRECT_HEREDOC:
////                // Heredocs should already be handled
////                break;
////        }
        
////        redirect = redirect->next;
////    }
    
////    return 1; // Success
////}

////// Function to clean up the redirection list (call during command cleanup)
////void free_redirections(t_redirect *redirections)
////{
////    t_redirect *current = redirections;
////    t_redirect *next;
    
////    while (current)
////    {
////        next = current->next;
////        if (current->filename)
////            free(current->filename);
////        // Close file descriptor if still open
////        if (current->fd != -1)
////            close(current->fd);
////        free(current);
////        current = next;
////    }
////}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/03/26 15:52:10 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stdio.h>

// Ambigious redirection
// export a="a b"
// > $a (bash: $a: ambiguous redirect)

// export a="a b"
// echo > $a | echo > $a | echo > $a | echo hi

// > $notexists
// bash: $notexists: ambiguous redirect

//echo <"./test_files/infile_big" | cat <"./test_files/infile" (18)
//cat <missing | cat <"./test_files/infile" (38)

//cat <minishell.h|ls (118) - parsing



int	execute_redirection(t_cmd *cmd, t_data *data,t_token **tokens)
{
	pid_t	pid;
	int		status;
	int		i;

(void) tokens;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), 0);
	if (pid == 0)
	{
		data->is_child = true;
		//set_child_signals();
		i = 0;
		while (data->input[i]) //need expan
		{
			if (data->input[i] == '>' && data->input[i + 1] == '>')
				handle_append_redirect(data, cmd);
			//else if (data->input[i] == '<' && data->input[i + 1] == '<')
			//	handle_heredoc(data);
			else if (data->input[i] == '<')
				handle_input_redirect(data, cmd);
			else if (data->input[i] == '>')
			{	
				//printf("im here\n");
				handle_output_redirect(data, cmd);
			}
			i++;
		}
		execute_for_one(tokens, cmd, data);
		//execute_command(cmd->args[0], data, cmd->args);
		//close(STDIN_FILENO);//
		//close(STDOUT_FILENO);//
		//printf("im here\n");
		exit(data->exit_status);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		///* Katya */
		//if (WEXITSTATUS(status) == 130 || WEXITSTATUS(status) == 131)
		//{
		//	if (WEXITSTATUS(status) == 131)
		//		printf("Quit (core dumped)");
		//	write (1, "\n", 1);
		//}
		///* End */
		// printf("exit status: %d\n", WEXITSTATUS(status));
		data->exit_status = WEXITSTATUS(status);
	}
	return (1);
}


void	apply_redirections(t_cmd *cmd, t_data *data)
{
	int	i;

	if (cmd->heredoc_delimiter && cmd->heredoc_fd != -1)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 heredoc");
			exit(1);
		}
		close(cmd->heredoc_fd);
	}
	i = 0;
	//printf("im here\n");
	while (data->input[i])
	{
		if (data->input[i] == '>' && data->input[i + 1] == '>')
			handle_append_redirect(data, cmd);
		else if (data->input[i] == '<' && data->input[i + 1] == '<')
			;
		else if (data->input[i] == '<')
			handle_input_redirect(data,cmd);
		else if (data->input[i] == '>')
			handle_output_redirect(data, cmd);
		i++;
	}
}