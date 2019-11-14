#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <unordered_map>

#include "../dark.hpp"
#include "../vertex/vertex.hpp"
#include "../texture/texture.hpp"

// store in file
const std::string MODEL_PATH = "/home/samurai/dark-engine/engine/source/Render/models/chalet.obj";
const std::string TEXTURE_PATH = "/home/samurai/dark-engine/engine/source/Render/textures/chalet.jpg";

class Object { // : public Asset {
	/*
	2d and 3d objects for now
	*/
public:
	Object() {}
	bool _Init(const std::string path);

	Texture *get_texture() {return texture;}
	std::vector<Vertex> get_vertices() {return vertices;}
	std::vector<uint32_t> get_indices() {return indices;}

	void load_model(const char *path);
	void load_object(const char *path);

private:
	Texture *texture;
	std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

};

#endif // OBJECT_HPP