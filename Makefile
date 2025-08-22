# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vkuusela <vkuusela@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/02 15:51:16 by vkuusela          #+#    #+#              #
#    Updated: 2025/08/22 12:31:34 by vkuusela         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 			= ircserv
CC 				= c++
CFLAGS 			= -Wall -Wextra -Werror -std=c++20
RM 				= rm -rf

SOURCES			= main.cpp RecvParser.cpp Client.cpp Server.cpp

HEADERS			= RecvParser.hpp Client.hpp Server.hpp

OBJECTS			= $(SOURCES:.cpp=.o)

%.o:			%.cpp $(HEADERS)
	@$(CC) $(CFLAGS) -c $< -o $@

all: 			$(NAME)

$(NAME):		$(OBJECTS) $(HEADERS)
	@echo -n "Compiling $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)
	@echo " Done."

clean:
	@echo -n "Cleaning .o files..."
	@$(RM) $(OBJECTS)
	@echo " Done."

fclean:			clean
	@echo -n "Full clean..."
	@$(RM) $(NAME)
	@echo " Done."

re:				fclean all

debug:			CFLAGS += -g
debug:			re

fsanitize:		CFLAGS += -g -fsanitize=address
fsanitize:		re

gprof:			CFLAGS += -g -pg
gprof:			re

.PHONY:			all clean fclean re debug fsanitize gprof
