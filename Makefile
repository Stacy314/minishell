NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline
SRC_DIR = src
OBJ_DIR = obj
SRC_FILES = main.c utils.c utils_init.c signals.c signals2.c initialization.c clean.c \
			execution/commands.c execution/commands_utils.c execution/pipe.c execution/execution.c execution/heredoc_expansion.c \
			execution/redirection.c execution/redir_handle.c execution/heredoc.c  execution/underscore.c execution/commands_utils2.c \
			execution/pipe_heredoc.c execution/utils_heredoc.c execution/heredoc_expansion2.c \
			execution/utils_commands.c execution/pipe2.c\
			parser/parser.c parser/parse_redirects.c \
			builtins/echo.c builtins/cd.c builtins/cd_utils.c builtins/env.c builtins/exit.c builtins/exit_utils.c \
			builtins/export.c builtins/export_print.c builtins/pwd.c builtins/unset.c \
			tokenizer/tokenizer.c tokenizer/utils_tokenizer.c tokenizer/find_operator.c tokenizer/handle_redir.c \
			tokenizer/expansion.c
OBJ_FILES = $(SRC_FILES:%.c=$(OBJ_DIR)/%.o)

MAKEFLAGS += --no-print-directory
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -c $< -o $@


$(NAME): $(OBJ_FILES)
	 @$(CC) $(CFLAGS) -o $(NAME) $(OBJ_FILES) $(LIBFT) $(LDFLAGS)

$(LIBFT):
	 @$(MAKE) -C $(LIBFT_DIR)

clean:
	 @rm -rf $(OBJ_DIR)
	 @$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	 @rm -f $(NAME)
	 @$(MAKE) -C $(LIBFT_DIR) fclean

re:  fclean all

.PHONY: all clean fclean re
