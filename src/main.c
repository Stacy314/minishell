/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
/*   Updated: 2024/12/18 21:57:39 by apechkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	(void)env;
    char *input;
	 
	//validation(argc, argv)
	//if (argc < 1)
	//	return(0);
    while (1) //
	{
        input = readline("minishell$ ");
        if (!input)
		{ 
            printf("\n");
			//free(input);
            break;
        }
		add_history(input);
        //printf("You entered: %s\n", input);
		split_to_token(input);
        free(input);
    }
	clear_history();
	return(0);
}
