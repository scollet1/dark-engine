// TextureAsset.cpp

bool TextureAsset::load_texture(const std::string path) {
	if (!(pixels = stbi_load(
		path,
		&tex_width, &tex_height,
		&tex_channels, STBI_rgb_alpha
	))) {
		throw std::runtime_error("failed to load texture image!");
	}

	return (SUCCESS);
}

bool TextureAsset::_Init() {
	return SUCCESS;
}

int TextureAsset::get_texture_size(/*int opt*/) {
	return tex_width * tex_height * 4;
}

int TextureAsset::get_tex_width() {
	return tex_width;
}

int TextureAsset::get_tex_height() {
	return tex_height;
}


bool TextureAsset::_Destroy(VkDevice *device) {
	vkDestroySampler(device, textureSampler, nullptr);
    vkDestroyImageView(device, textureImageView, nullptr);
    vkDestroyImage(device, textureImage, nullptr);
    vkFreeMemory(device, textureImageMemory, nullptr);
}

VkImage TextureAsset::get_texture_image() {
	return texture_image;
}