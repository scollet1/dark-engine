NAME = DarkEngine

TEST := $(if $(TARGET),$(TARGET),RUN)

CC = g++

SRC_DIR = source
INC_DIR = includes

# -fsanitize=address
ARCHITECTURE = x86_64
# TODO : make dynamic
# configurable from install file
VK_VERSION = 1.1.92.1
VK_PATH = ./VulkanSDK/$(VK_VERSION)/$(ARCHITECTURE)
STB_INCLUDE_PATH = ./stb/
TOBJ_INCLUDE_PATH = ./tiny_obj_loader

INCL = -I $(INC_DIR) -I$(VK_PATH)/include/ -I$(STB_INCLUDE_PATH) -I$(TOBJ_INCLUDE_PATH)

CFLAGS  = -Wall -Wextra -g # -Werror 

LDFLAGS = -L $(VK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

# MINILIB = ./mini_lib/

# SRC_FILES = testing/testengine.cpp
SRC_FILES = Engine/DarkEngine.cpp \
			Engine/Environ.cpp \
			Render/RenderManager.cpp \
			Render/Debug.cpp \
			Render/Devices.cpp \
			Render/Images.cpp \
			Render/Shaders.cpp \
			Render/Surface.cpp \
			Render/SwapChain.cpp \
			Render/Validation.cpp \
			texture/texture.cpp \
			main.c

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