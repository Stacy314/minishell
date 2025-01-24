/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/24 22:45:49 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void print_cmd_list(t_cmd *cmd_list, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        printf("=== Command %zu ===\n", i);
        
        // Print args
        printf("args: ");
        if (cmd_list[i].args)
        {
            size_t arg_index = 0;
            while (cmd_list[i].args[arg_index])
            {
                printf("\"%s\" ", cmd_list[i].args[arg_index]);
                arg_index++;
            }
        }
        else
        {
            printf("(none)");
        }
        printf("\n");
        
        // Print redirections
        printf("input_redirect:   %s\n",
               cmd_list[i].input_redirect ? cmd_list[i].input_redirect : "(none)");
        printf("output_redirect:  %s\n",
               cmd_list[i].output_redirect ? cmd_list[i].output_redirect : "(none)");
        printf("append_redirect:  %s\n",
               cmd_list[i].append_redirect ? cmd_list[i].append_redirect : "(none)");
        printf("heredoc_delimiter:%s\n",
               cmd_list[i].heredoc_delimiter ? cmd_list[i].heredoc_delimiter : "(none)");
        
        // Print pipe fields
        printf("pipe_in:          %d\n", cmd_list[i].pipe_in);
        printf("pipe_out:         %d\n", cmd_list[i].pipe_out);

        // Print data if relevant
        if (cmd_list[i].data)
        {
            // Adjust depending on your t_data structure
            printf("data->some_value: %d\n", cmd_list[i].data->some_value);
        }
        else
        {
            printf("data:            (null)\n");
        }
        
        printf("\n");
    }
}



int		main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
    char	*input;
	t_data	data;
	t_token	**tokens;
	t_cmd	*cmd;
	
	if (argv && argc > 1)
	{
		ft_putstr_fd("Minishell cannot take arguments\n", STDOUT_FILENO);
		return (EXIT_FAILURE);
	}
	init_structure(&data, env);
	signal_handler(); //(CTRL+C, CTRL+D, CTRL+\)
	
    while (1) //
	{

        input = readline("minishell$ ");
        if (!input)
		{ 
            printf("\n");
			//free(input);
            break;
        }
		if(*input)
			add_history(input); // need to handel
        //printf("You entered: %s\n", input);
		
		tokens = split_to_tokens(input);
		free(input);
		if (!tokens)
		{
            fprintf(stderr, "Error: Failed to tokenize input\n");
            continue;
		}
		cmd = parse_tokens(tokens);
        if (!cmd)
		{
            ft_putendl_fd("Error: Failed to parse tokens", STDERR_FILENO);
            exit(EXIT_FAILURE); //need to check
        }
		print_cmd_list(cmd,2);
		//int i = 0;
		//while(cmd->args[i])
		//{
		//	printf("args: %s\n", cmd->args[i]);
		//	cmd->args++;
		//	if (cmd->args[i] == '|' || cmd->args[i] == '<' || cmd->args[i] == '>')
		//		break;
		//}
		//printf("%d", data.exit_status);
		
				
		//if (contains_special_char(cmd, '<') || contains_special_char(cmd, '>'))
		if (execute_redirection(cmd,env) == 1)
			continue;
		if (contains_special_char(cmd, '|'))
		{
			//execute_for_many(tokens, cmd);
			execute_pipeline(&cmd, data.env);
			//printf("execute for many run\n");
		}
		else
		{
    		execute_for_one(tokens, cmd, &data);
		}

		 // need ecsept for builtins
		free(cmd);
		free_tokens(tokens);
    }
	clear_history();
	return(data.exit_status);
}


