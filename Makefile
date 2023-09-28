# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dreis-ma <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/01 18:12:34 by dreis-ma          #+#    #+#              #
#    Updated: 2023/09/28 18:02:21 by dreis-ma         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -pthread -Wall -Wextra -Werror -g
DEPS = src/philo.h

SRC = src/main.c src/utils.c src/init.c src/routine.c src/routine_utils.c

OBJ = $(SRC:.c=.o)

NAME = philo

all: $(NAME)

$(NAME): $(OBJ) $(DEPS)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all
