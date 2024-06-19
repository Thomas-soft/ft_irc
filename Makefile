CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

# Normal
NAME = ircserv
FUNC = ft_irc.cpp
OBJ = $(patsubst srcs/%.cpp, obj/%.o, $(FUNC))

HEADER = ft_irc.hpp
# Compil
all: 	${NAME}

$(OBJ): ${HEADER}

${NAME}: ${OBJ}
	${CC} ${OBJ} $(CFLAGS) -o $(NAME)

obj/%.o: srcs/%.cpp
	@mkdir -p obj/
	$(CC) $(CFLAGS) -c $< -o $@

clean:	
	rm -rf obj

fclean:	clean
	rm -f ${NAME}

re:	fclean all

.PHONY: all clean fclean re bonus
