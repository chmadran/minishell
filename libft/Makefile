# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chmadran <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/10 16:46:56 by chmadran          #+#    #+#              #
#    Updated: 2023/07/22 14:58:39 by chmadran         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = gcc
CFLAGS += -Wall -Wextra -Werror
RM = rm -f

MY_SOURCES = ft_atoi.c \
		     ft_calloc.c \
			 ft_itoa.c \
			 ft_memcpy.c \
			 ft_memmove.c \
			 ft_memset.c \
			 ft_putchar_fd.c \
			 ft_putstr_fd.c \
			 ft_strchr.c \
			 ft_strcmp.c \
			 ft_strdup.c \
			 ft_strjoin.c \
			 ft_strlcat.c \
			 ft_strlcpy.c \
			 ft_strlen.c \
			 ft_strmapi.c \
			 ft_strncmp.c \
			 ft_strndup.c \
			 ft_strnstr.c \
			 ft_strrchr.c \

MY_OBJECTS = $(MY_SOURCES:.c=.o)



BONUS_OBJECTS = $(BONUS:.c=.o)

all: $(NAME)

$(NAME): $(MY_OBJECTS)
	ar -rcs $(NAME) $(MY_OBJECTS)
so:
	$(CC) -nostartfiles -fPIC $(CFLAGS) $(MY_SOURCES) $(BONUS)
	$(CC) -nostartfiles -shared -o libft.so $(MY_OBJECTS) $(BONUS_OBJECTS)

clean:
	$(RM) $(MY_OBJECTS) $(BONUS_OBJECTS)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)
