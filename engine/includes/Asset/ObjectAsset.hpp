class ObjectAsset : Asset {
public:
	bool load_model(const std::string path);
	bool load_texture(const std::string path);

private:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	TextureAsset *texture;	
}