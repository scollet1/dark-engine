#include "../../includes/Scene/SceneManager.hpp"

void SceneManager::transition_to(int new_scene_id) {
	//
	Scene new_scene;
	new_scene = scenes.get_node(new_scene_id);
	current_scene = new_scene;
	new_scene.transitioned();
	// begin domino effect
}

void SceneManager::load_scene(SceneDescriptor s) {
	Scene scene;

	scene = scenes[s];
	
	load_assets(scene);
	if (dark_engine.get_options().optimzations.preload_scenes) {
		auto neighbors = boost::adjacent_vertices(s, scenes);

		for (auto n : make_iterator_range(neighbors)) {
			/*
			perform is_loaded?
			check on asset load
			*/
			preload_assets(n);
		}
	}
}

void SceneManager::load_scene_graph(const std::string path) {
	/*
	SceneDescriptor s_desc;
	std::vector<Scene> resolve_list;

	with open path + scenes as scenes_directory {
		for scene in scenes_directory {
			// need to be serialized in order
			s_desc = boost::add_vertex(scenes);
			resolve_list.push_back(s_desc);
			// create_scene(s_desc);
		}
	}

	with open path + transitions as transitions_directory {
		for transition in transitions_directory {
			for edges in transition {
				boost::add_edge(
					resolve_list[transition.a],
					resolve_list[transition.b],
					scenes
				);
			}
		}
	}

	current_scene = resolve_list[0];
	load_scene(current_scene);
	*/
}