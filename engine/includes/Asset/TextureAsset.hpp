class TextureAsset : Asset {
public:
	TextureAsset() {}
	TextureAsset(const std::string path) {
		if (!load_texture(path)) {
			return (NULL); // do something
		} else {
			loaded = true;
		}
	}

	bool _Destroy();

	bool load_texture(const std::string path);
	VkImage get_texture_image();
	int get_texture_size();
	int get_tex_width();
	int get_tex_height();

private:
	VkImage texture_image;
	stbi_uc *pixels;
	int tex_width;
	int tex_height;
	int tex_channels;
}