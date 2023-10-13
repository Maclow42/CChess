CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -O3
LDFLAGS =
LDLIBS =

SRC = 	main.c \
		./src/piece/piece.c \
		./src/gameboard/gameboard.c \
		./src/gameboard/accessible_pos.c \
		./src/gameboard/game_status.c \
		./src/gameboard/move_piece.c \
		./src/gameboard/state_backuper.c \
		./src/coords/coords.c \
		./src/minmax/minmax.c \
		./src/macros/usefull.c \
		./src/list/list.c \
		./src/list/list_node.c \
		./src/list/list_iterator.c \
		./src/gtree/gtree.c \
		./src/stack/stack.c \
		./src/stack/stack_node.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: ${OBJ}
	${CC} ${LDFLAGS} -o $@ $^ ${LDLIBS}

%.o: %.c
	${CC} ${CPPFLAGS} ${CFLAGS} -c -o $@ $<

-include ${DEP}

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main
