/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/21 12:28:27 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		
        //if (!cmd)
		//{
        //    //fprintf(stderr, "Error: Failed to parse tokens\n");
        //    continue;
        //}
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
		if (execute_redirection(cmd,env) == 1)
			continue;
		 // need ecsept for builtins
		free(cmd);
		free_tokens(tokens);
    }
	clear_history();
	return(data.exit_status);
}


