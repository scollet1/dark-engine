NAME = TopoEngine

TESTING = TopoEngine.test

CC = g++

SRC_DIR = source

CFLAGS  = -Wall -Werror -Wextra -g
# -fsanitize=address

INCL = -I includes

# MINILIB = ./mini_lib/

SRC_FILES = engine/Engine.cpp   \
			engine/Environ.cpp  \
			engine/game/Game.cpp\
			utils/memory.cpp    \
			render/RenderMgr.cpp\
			main.cpp
#			topo.cpp

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

RM = rm -fr

$(NAME):
		$(CC) $(CFLAGS) $(INCL) $(SRC) -o $(NAME)

$(TESTING):
		$(CC) $(CFLAGS) $(INCL) $(SRC) -D TESTING -o $(NAME)

all: $(NAME)

test: $(TESTING)

clean:
		$(RM) $(OBJDIR)

fclean: clean
		$(RM) $(NAME)

re: fclean $(NAME)