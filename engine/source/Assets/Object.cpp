#include "../includes/Assets/Object.hpp"

void Object::load_model(const char *path) {
	/*
	parallelize whatever parent is calling this
	*/

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(
    	&attrib, &shapes, &materials,
    	&warn, &err, path)
	) {
        throw std::runtime_error(warn + err);
    }

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
	for (const auto& shape : shapes) {
	    for (const auto& index : shape.mesh.indices) {
	        Vertex vertex = {};
	        
            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = {1.0f, 1.0f, 1.0f};

            // only load the vertices once
	        if (uniqueVertices.count(vertex) == 0) {
	            uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
	            vertices.push_back(vertex);
	        }

	        indices.push_back(uniqueVertices[vertex]);
	    }
	}
}

void Object::load_object(const std::string path) {
	/*
	with open path as file {
		load_model(file.model_path);
		texture = load_texture(file.texture_path);
	}
	*/

	(void)path;

	load_model(MODEL_PATH.c_str());
	texture = load_texture(TEXTURE_PATH.c_str());
}

bool Object::_Init(const std::string path) {
	load_object(path);
	return SUCCESS;
}