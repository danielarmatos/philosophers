# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dreis-ma <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/01 18:12:34 by dreis-ma          #+#    #+#              #
#    Updated: 2023/09/20 20:12:37 by dreis-ma         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -pthread -Wall -Wextra -Werror -g
#CFLAGS = -Wall -Wextra -Werror -pthread -g3 -fsanitize=thread
DEPS = src/philo.h

SRC = src/main.c src/utils.c src/init.c src/routine.c

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