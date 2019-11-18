#include "../../includes/Scene/SceneManager.hpp"

void SceneManager::load_assets_to_job_queue(Scene *scene, bool deferred) {
	/*
	@defererred:
		false: loose scheduling, job can be completed "in the future"
		true: tight scheduling, job should be completed "now"
	*/
	// float priority;

	// if (deferred) {
	// 	priority = 0.0;
	// } else {
	// 	priority = 1.0;
	// }

	// dark_engine.get_job_queue().insert_with_priority(
	// 	scene.load_assets,
	// 	scene.load_assets_callback,
	// 	priority
	// );
}

void SceneManager::preload_assets(Scene *scene) {
	load_assets_to_job_queue(scene, true);
}

void SceneManager::load_assets(Scene *scene) {
	load_assets_to_job_queue(scene, false);
}

void SceneManager::transition_to(SceneDescriptor new_scene) {
	// //
	// Scene new_scene;
	// new_scene = scenes.get_node(new_scene_id);
	// current_scene = new_scene;
	// new_scene.transitioned();
	// begin domino effect
}

bool SceneManager::load_scene(SceneDescriptor s) {
	if (scenes[s].load_assets() == FAILURE) return FAILURE;
	assert(scenes[s].get_all_objects().size());

	// if (dark_engine.get_options().optimzations.preload_scenes) {
	// 	auto neighbors = boost::adjacent_vertices(s, scenes);

	// 	for (auto n : make_iterator_range(neighbors)) {
	// 		/*
	// 		perform is_loaded?
	// 		check on asset load
	// 		*/
	// 		preload_assets(n);
	// 	}
	// }
	return SUCCESS;
}

void SceneManager::load_scene_graph(const std::string path) {
	/*
	SceneDescriptor s_desc;
	std::vector<SceneDescriptor> resolve_list;

	with open path + scenes as scenes_directory {
		for scene in scenes_directory {
			// need to be serialized in order
			s_desc = boost::add_vertex(scene);
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
	(void)path;
	SceneDescriptor s_desc = boost::add_vertex(scenes);
	boost::add_edge(s_desc, s_desc, scenes);
	if (load_scene(s_desc) == FAILURE) {
		throw std::runtime_error("scene failed to load!");
	}
	current_scene = s_desc;
}

// void SceneManager::create_test_scene() {
// 	printf("new test object\n");
// 	test_object->load_object(
// 		"Testing path does not matter \
// 		just need to trip the constructor"
// 	);
// 	printf("new test object loaded\n");
// 	test_scene->add_test_object_to_scene(*test_object);
// 	printf("added new test object\n");
// 	test_scenes.push_back(test_scene);
// }