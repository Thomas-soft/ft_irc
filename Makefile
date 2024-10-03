# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dmeurant <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 09:08:49 by dmeurant          #+#    #+#              #
#    Updated: 2023/11/27 09:08:50 by dmeurant         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= irc

SRCS		=	src/main.cpp \
				src/Server.cpp \
				src/Client.cpp \
				src/Commands.cpp
				

OBJS		= $(SRCS:.cpp=.o)
RM			= rm -f
FLAGS		= -Wall -Wextra -Werror -std=c++98
COMPILER	= c++ 

.cpp.o:
	${COMPILER} ${FLAGS} -c $< -o ${<:.cpp=.o}

$(NAME): ${OBJS}
	${COMPILER} ${FLAGS} $(OBJS) -o $(NAME)

all: $(NAME)
    
fclean: clean
	$(RM) $(NAME)
    
clean:
	$(RM) -f $(OBJS) $(OBJS_B)
    
re: fclean all

.PHONY: all clean fclean re .cpp.o
