SRC	=	src/main.c\
		src/str2arr.c\
		src/util.c

BIN	=	top

OBJ	=	$(SRC:.c=.o)

CFLAGS	+=	-Wall -Werror -Wextra -g3 -lncurses

CC	=	gcc

RM	=	rm -rf

all:	$(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(BIN)

re: fclean all

.PHONY: all clean fclean re
