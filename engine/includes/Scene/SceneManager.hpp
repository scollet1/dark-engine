#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

typedef boost::adjacency_list <
	boost::listS,
	boost::vecS,boost::undirectedS,
	boost::no_property,
	Scene, TransitionEdge
> SceneGraph;

typedef boost::graph_traits<Graph>::vertex_descriptor SceneDescriptor;
typedef boost::graph_traits<Graph>::edge_descriptor TransitionDescriptor;

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
	void load_assets_from_job_queue(Scene *obj, bool deferred
		/*
			dark_engine.job_queue.submit_work(
				obj.load_assets,
				obj.load_assets_callback
			);
		*/
	);
	void load_assets(Scene *obj, false);
	void preload_assets(Scene *obj, true);
	Scene *get_current_scene() {return current_scene;}

private:
	void load_scene_graph(const std::string path);
	void transition_to();
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
}

#endif // SCENEMANAGER_HPP