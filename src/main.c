/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2025/01/07 16:33:35 by apechkov         ###   ########.fr       */
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
	 

	data.exit_status = 0;  //change
	//validation(argc, argv)
	//if (argc < 1)
	//	return(0);
    while (1) //
	{
        input = readline("minishell$ ");
        if (!input)
		{ 
            printf("\n");
			// Handle CTRL+D or EOF
			//free(input);
            break;
        }
		add_history(input);
        //printf("You entered: %s\n", input);
		tokens = split_to_tokens(input);
		//if (!tokens)
		//{
        //    fprintf(stderr, "Error: Failed to tokenize input\n");
        //    free(input);
        //    continue;
        //}
		//execute(tokens, &data);
		//free_tokens(tokens);
		free(input);
    }
	clear_history();
	return(data.exit_status);
}


