NAME = TopoEngine

TESTING = TopoEngine.test

CC = g++

SRC_DIR = source

CFLAGS  = -Wall -Werror -Wextra -g
#-fsanitize=address

INCL = -I includes

#MINILIB = ./mini_lib/

SRC_FILES = engine/Engine.cpp   \
			engine/Environ.cpp  \
			engine/game/Game.cpp\
			error/
			utils/memory.cpp    \
			main.cpp            \
			topo.cpp

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

RM = rm -fr

$(NAME):
		$(MAKE) -C $(MINILIB) re
		$(CC) $(CFLAGS) $(INCL) $(SRC) $(MINILIB)/mini_lib.a -o $(NAME)

$(TESTING):
		$(MAKE) -C $(MINILIB) re
		$(CC) $(CFLAGS) $(INCL) $(SRC) $(MINILIB)/mini_lib.a -D TESTING -o $(NAME)

all: $(NAME)

test: $(TESTING)

clean:
		$(RM) $(OBJDIR)

fclean: clean
		$(RM) $(NAME)

re: fclean $(NAME)