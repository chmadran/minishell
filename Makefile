# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chmadran <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/21 15:11:11 by chmadran          #+#    #+#              #
#    Updated: 2023/07/22 15:02:40 by chmadran         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name of the executable
NAME		:= minishell

# Directories
SRC_DIR		:= ./src
BUILD_DIR	:= ./build
INCLUDE_DIR	:= ./include
LIBFT_DIR	:= ./libft

# Source, object, dependency files
SRC_FILES	:= $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/**/*.c)
OBJ_FILES	:= $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Compiler flags
CFLAGS		:= -Wall -Wextra -Werror
INCLUDES	:= -I$(INCLUDE_DIR) -I$(LIBFT_DIR)/include

# Linker flags
LDFLAGS		:= -L$(LIBFT_DIR)
LDLIBS		:= -lft

# Colors
GREEN	=	\033[1;32m
NO_COLOR	=	\033[0m

# Phony targets
.PHONY: all lib clean fclean re cleanlib fcleanlib relib fcleanall

# Default target
all: lib $(NAME)

# Target to build the lib
lib:
	$(MAKE) -C $(LIBFT_DIR)

# Target to create directories automatically, compile src files into obj files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(LDFLAGS) $(LDLIBS) -o $(NAME)
	@echo "\n$(GREEN)EXECUTABLE READY.$(NO_COLOR)"

# Target to clean the project by removing the directory build
clean:
	@rm -rf $(BUILD_DIR)
	@echo "\n$(GREEN)OBJECT FILES DELETED.$(NO_COLOR)\n"

# Target to remove the executable
fclean: clean
	@rm -rf $(NAME)
	@echo "\n$(GREEN)EXECUTABLE DELETED.$(NO_COLOR)\n"

# Target to clean the lib
cleanlib:
	$(MAKE) clean -C $(LIBFT_DIR)
	@echo "\n$(GREEN)LIBRARY OBJECTS DELETED.$(NO_COLOR)\n"

# Target to remove the libft project by removing the directory build
fcleanlib:
	$(MAKE) fclean -C $(LIBFT_DIR)
	@echo "\n$(GREEN)LIBRARY EXECUTABLE DELETED.$(NO_COLOR)\n"

# Target to rebuild the project from scratch
re: fclean all

.PHONY: all lib clean fclean cleanlib fcleanlib re