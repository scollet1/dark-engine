#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include "./Scene.hpp"
#include "../Assets/Object.hpp"

struct SceneEdge {
	int place_holder;
}

typedef boost::adjacency_list<
	boost::listS,
	boost::vecS,boost::undirectedS,
	boost::no_property,
	Scene, SceneEdge
>SceneGraph;

typedef boost::graph_traits<SceneGraph>::vertex_descriptor SceneDescriptor;
typedef boost::graph_traits<SceneGraph>::edge_descriptor TransitionDescriptor;

class SceneManager {
	/*
	SceneManager is responsible for traversing the
	Scenes folder and compiling pointers to the Scene
	Graph. So each Scene will be created but that Scene's
	Assets will be unloaded. I think the edges of the
	current root node(i.e. the current Scene), should be
	partially or full initialized, depending on memory
	constraints.

	Perhaps the developer will provide hints to dynamically
	preload these Scenes for smoother transitions and shorter
	buffer time
	*/

	/*
	neighbors is a list of neighbor nodes with optional
	preloading
	*/
public:
	Scene *get_current_scene() {return scenes[current_scene];}
	Scene *get_current_test_scene() {return test_scene;}

	void load_assets_to_job_queue(Scene *scene, bool deferred
		/*
			dark_engine.job_queue.submit_work(
				obj.load_assets,
				obj.load_assets_callback
			);
		*/
	);
	void load_scene(SceneDescriptor s);
	void load_assets(Scene *scene);
	void preload_assets(Scene *scene);
	void create_test_scene();

private:
	void load_scene_graph(const std::string path);
	void transition_to(SceneDescriptor new_scene);
	/*
	here I'm not so sure we'll always be able to
	explicitily declare a scene graph. The reason
	being: developers shouldn't necessarily be
	coerced into manually structuring a scene graph
	with deterministic edges for transitions. If we can
	structure it this way, however, it makes our
	work a lot easier, esp. for optimizations
	Graph *scenes;
	*/
	SceneGraph scenes;
	SceneDescriptor *current_scene;
	Scene *test_scene;
};

#endif // SCENEMANAGER_HPP