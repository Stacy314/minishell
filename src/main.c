/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/09 16:34:48 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	(void)env;
    char	*input;
	t_data	data;
	t_token	**tokens;
	 

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
		//if(*input)
		add_history(input); // need to handel
        //printf("You entered: %s\n", input);
		
		tokens = split_to_tokens(input);
		if (!tokens)
		{
            fprintf(stderr, "Error: Failed to tokenize input\n");
            free(input);
            continue;
        }
		//printf("before execute\n");
		execute(tokens, &data);
		//free_tokens(tokens);
		//free(input);
    }
	clear_history();
	return(data.exit_status);
}


