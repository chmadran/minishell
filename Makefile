# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chmadran <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/21 15:11:11 by chmadran          #+#    #+#              #
#    Updated: 2023/07/21 15:36:31 by chmadran         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#-------------------VARIABLES---------------------
NAME		= minishell
COMP		= gcc
CFLAGS		= -Wall -Werror -Wextra

#-------------------DIRECTORIES-------------------

SRC_DIR		:= ./src
BUILD_DIR	:= ./build
INCLUDE_DIR	:= ./include
LIBFT_DIR	:= ./lib/libft

#---------SOURCE, OBJECT, DEPENDENCY, FILES-------
#The specificities are explained in the Makefile

SRC_FILES	:= $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/**/*.c)
OBJ_FILES	:= $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

#-------------------COLORS------------------------

GREEN 		= \033[32m
NO_COLOR	= \033[0m

#-------------------RULES------------------------

all: $(NAME)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(LDFLAGS) $(LDLIBS) -o $(NAME)
	@echo "\n$(GREEN)EXECUTABLE READY.$(NO_COLOR)"

clean:
	@rm -rf $(BUILD_DIR)
	@echo "\n$(GREEN)OBJECT FILES DELETED.$(NO_COLOR)"

fclean: clean
	@rm -rf $(NAME)
	@echo "\n$(GREEN)EXECUTABLE DELETED.$(NO_COLOR)"

re: fclean all

.PHONY: all object clean fclean re