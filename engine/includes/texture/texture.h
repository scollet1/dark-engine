#ifndef TEXTURE_H
#define TEXTURE_H

#include "../dark.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

typedef struct s_texture {
	std::string path;
	stbi_uc *pixels;
	int width;
	int height;
	int channels;
} Texture;

Texture *load_texture(const char *path);

#endif // TEXTURE_H