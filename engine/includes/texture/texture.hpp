#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "../dark.hpp"

typedef struct s_texture {
	char *path;
	stbi_uc *pixels;
	int width;
	int height;
	int channels;
} Texture;

Texture *load_texture(const char *path);

#endif // TEXTURE_HPP