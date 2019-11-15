#include "../includes/texture/texture.hpp"

Texture *load_texture(const char *path) {
	Texture *texture;

	if (!(texture = (Texture*)malloc(sizeof(Texture)))) return NULL;
	printf("what\n");
	size_t len = strlen(path);
	texture->path = (char*)malloc(sizeof(char) * len);
	printf("what\n");
	memmove(texture->path, path, len);
	printf("what\n");
	texture->pixels = stbi_load(
		path, &texture->width, &texture->height,
		&texture->channels, STBI_rgb_alpha
	);
	printf("what\n");

	if (!texture->pixels) {
		throw std::runtime_error("failed to load texture image!");
	}
	printf("what\n");

	return texture;
}