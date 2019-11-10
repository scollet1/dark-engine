#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "../texture/texture.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

// store in file
const std::string MODEL_PATH = "/home/samurai/dark-engine/engine/source/render/models/chalet.obj";
const std::string TEXTURE_PATH = "/home/samurai/dark-engine/engine/source/render/textures/chalet.jpg";

class Object { // : public Asset {
	/*
	2d and 3d objects
	*/
public:
	Object() {}
	Object(const std::string path) {
		load_object(path);
	}

	Texture *get_texture() {return texture;}
	std::vector<Vertex> get_vertices() {return vertices;}
	std::vector<uint32_t> get_indices() {return indices;}

	void load_model(const std::string path);
	void load_object(const std::string path);

private:
	Texture *texture;
	std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

};

#endif // OBJECT_HPP