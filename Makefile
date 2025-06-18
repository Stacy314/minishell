################################################################################
#                                                                              #
#	                                Vars                                       #
#                                                                              #
################################################################################

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline
SRC_DIR = src
OBJ_DIR = obj
SRC_FILES = main.c utils.c utils2.c signals.c signals2.c initialization.c clean.c \
			execution/commands.c execution/commands_utils.c execution/pipe.c execution/execution.c execution/heredoc_expansion.c \
			execution/redirection.c execution/redir_handle.c execution/heredoc.c  execution/underscore.c execution/commands_utils2.c \
			execution/pipe_heredoc.c execution/heredoc2.c execution/heredoc_expansion2.c \
			execution/commands_utils3.c execution/pipe2.c \
			parser/parser.c parser/parse_redirects.c parser/utils_parse_redir.c parser/utils_parser.c \
			builtins/echo.c builtins/cd.c builtins/cd_utils.c builtins/env.c builtins/exit.c builtins/exit_utils.c \
			builtins/export.c builtins/export_print.c builtins/pwd.c builtins/unset.c \
			tokenizer/tokenizer.c tokenizer/utils_tokenizer.c tokenizer/find_operator.c tokenizer/handle_redir.c \
			tokenizer/expansion.c tokenizer/utils_checks.c tokenizer/utils_exp.c tokenizer/utils_handle_word.c \
			tokenizer/utils_tokenizer2.c 
OBJ_FILES = $(SRC_FILES:%.c=$(OBJ_DIR)/%.o)

MAKEFLAGS += --no-print-directory
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SILENT = @

################################################################################
#                                                                              #
#	                                Rules                                      #
#                                                                              #
################################################################################

all: $(LIBFT) $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(SILENT)@mkdir -p $(dir $@)
	$(SILENT)@$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -c $< -o $@


$(NAME): $(OBJ_FILES)
	$(SILENT)@$(CC) $(CFLAGS) -o $(NAME) $(OBJ_FILES) $(LIBFT) $(LDFLAGS)
	@echo "███╗░░░███╗██╗███╗░░██╗██╗░██████╗██╗░░██╗███████╗██╗░░░░░██╗░░░░░"
	@echo "████╗░████║██║████╗░██║██║██╔════╝██║░░██║██╔════╝██║░░░░░██║░░░░░"
	@echo "██╔████╔██║██║██╔██╗██║██║╚█████╗░███████║█████╗░░██║░░░░░██║░░░░░"
	@echo "██║╚██╔╝██║██║██║╚████║██║░╚═══██╗██╔══██║██╔══╝░░██║░░░░░██║░░░░░"
	@echo "██║░╚═╝░██║██║██║░╚███║██║██████╔╝██║░░██║███████╗███████╗███████╗"
	@echo "╚═╝░░░░░╚═╝╚═╝╚═╝░░╚══╝╚═╝╚═════╝░╚═╝░░╚═╝╚══════╝╚══════╝╚══════╝"
$(LIBFT):
	$(SILENT)@$(MAKE) -C $(LIBFT_DIR)

clean:
	$(SILENT)@rm -rf $(OBJ_DIR)
	$(SILENT)@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(SILENT)@rm -f $(NAME)
	$(SILENT)@$(MAKE) -C $(LIBFT_DIR) fclean

re:  fclean all

.PHONY: all clean fclean re
