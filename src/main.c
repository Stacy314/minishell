/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/27 18:52:47 by apechkov         ###   ########.fr       */
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
    char	*input;
	t_data	data;
	t_token	**tokens;
	t_cmd	*cmd;
	
	if (argv && argc > 1)
	{
		ft_putstr_fd("Minishell cannot accept arguments\n", STDOUT_FILENO);
		return (EXIT_FAILURE);
	}
	cmd = init_structure(&data, env);
	signal_handler(); //(CTRL+C, CTRL+D, CTRL+\)
    while (1)
	{
		//printf("exit status in main start - %d\n", data.exit_status);
        input = readline("minishell$ ");
        if (!input)
		{ 
            printf("\n");

            exit(EXIT_FAILURE);
        }
		if(*input)
			add_history(input);
		tokens = split_to_tokens(input);
		free(input);
		if (!tokens)
		{
            printf("Error: Failed to tokenize input\n");
            continue;
		}
		cmd = parse_tokens(tokens);
        if (!cmd)
		{
            ft_putendl_fd("Error: Failed to parse tokens", STDERR_FILENO);
            exit(EXIT_FAILURE);
        }
		
		////////////////////////
		print_cmd_list(cmd,4);
		
		if (cmd->heredoc_delimiter || cmd->input_redirect || cmd->output_redirect || cmd->append_redirect)
		{
			if (execute_redirection(cmd,env) == 1)
				continue;
		}
		if (contains_special_char(tokens, '|'))
		{
			execute_pipeline(&cmd, &data, data.env);
		}
		else
			execute_for_one(tokens, cmd, &data);
	//	free(cmd);
		free_cmd(cmd);
		free_tokens(tokens);
		//printf("exit status in main end - %d\n", data.exit_status);
    }
	clear_history();
	//free_cmd(cmd);
	//rl_clear_history();
	return(data.exit_status);
}


