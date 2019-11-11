#include "../includes/texture/texture.hpp"

Texture *load_texture(const char *path) {
	Texture *texture;

	if (!(texture = (Texture*)malloc(sizeof(Texture)))) return NULL;

	texture->path = (char*)malloc(sizeof(path) * strlen(path));
	memcpy(&texture->path, path, strlen(path));
	texture->pixels = stbi_load(
		path, &texture->width, &texture->height,
		&texture->channels, STBI_rgb_alpha
	);

	if (!texture->pixels) {
		throw std::runtime_error("failed to load texture image!");
	}

	return texture;
}