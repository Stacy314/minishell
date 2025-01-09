NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline
SRC_DIR = src
OBJ_DIR = obj
SRC_FILES = main.c tokenizer.c utils_tokenizer.c builtins.c utils_builtins.c \
			execute.c parser.c signals.c initialization.c
OBJ_FILES = $(SRC_FILES:.c=.o)
OBJS =  $(OBJ_DIR)/main.o $(OBJ_DIR)/tokenizer.o  $(OBJ_DIR)/utils_tokenizer.o \
		$(OBJ_DIR)/builtins.o $(OBJ_DIR)/utils_builtins.o $(OBJ_DIR)/execute.o \
		$(OBJ_DIR)/parser.o $(OBJ_DIR)/signals.o $(OBJ_DIR)/initialization.o
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -c $< -o $@

$(OBJ_DIR): 
	mkdir -p $(OBJ_DIR)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all