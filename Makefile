CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -O3
LDFLAGS =
LDLIBS =

SRC = main.c piece.c board.c coords.c alpha_beta.c ./src/list/list.c ./src/list/list_node.c ./src/list/list_iterator.c ./src/tree/tree.c ./src/stack/stack.c ./src/stack/stack_node.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: ${OBJ}

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main