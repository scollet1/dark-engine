NAME = DarkEngine

TEST := $(if $(TARGET),$(TARGET),RUN)

CC = g++

SRC_DIR = source
INC_DIR = includes

# -fsanitize=address
ARCHITECTURE = x86_64
# TODO : make dynamic
VK_VERSION = 1.1.92.1
VK_PATH = ./VulkanSDK/$(VK_VERSION)/$(ARCHITECTURE)
STB_INCLUDE_PATH = ./stb/

INCL = -I $(INC_DIR) -I$(VK_PATH)/include/ -I$(STB_INCLUDE_PATH)

CFLAGS  = -Wall -Wextra -g # -Werror 

LDFLAGS = -L $(VK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

# MINILIB = ./mini_lib/

# SRC_FILES = testing/testengine.cpp
SRC_FILES = engine/Engine.cpp   \
			engine/Environ.cpp  \
			engine/game/Game.cpp\
			utils/memory.cpp    \
			threads/ThreadPool.cpp \
			render/RenderMgr.cpp    \
			render/Debug.cpp        \
			render/Devices.cpp      \
			render/Shaders.cpp      \
			render/Surface.cpp      \
			render/SwapChain.cpp    \
			render/Validation.cpp   \
			main.cpp

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

RM = rm -fr

$(NAME):
		echo $(TEST)
		$(CC) $(CFLAGS) $(INCL) $(SRC) $(LDFLAGS) -o $(NAME) -D $(TEST)

all: $(NAME)

clean:
		$(RM) $(OBJDIR)

fclean: clean
		$(RM) $(NAME)

re: fclean $(NAME)