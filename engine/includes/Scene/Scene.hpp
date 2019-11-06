class Scene {
public:
	/*
	When the scene is triggered to load/prepped for
	transition, it will traverse its configuration
	file to collect pointers to each asset file.

	Each Asset should submit itself to an init process
	that returns a pointer to the scene, asset list

	debate whether or not to hand this to a threadpool...

	consider, on asset load, loading that asset once
	for the first time and then signaling to all scenes
	with a pointer to that asset, that it had in fact
	been loaded and they do not need to reload it
	*/

	/*
	how do we explicitly define the asset dependencies?
	*/
	Scene() {
		all_assets_loaded = false;
		ready_to_transition = false;
	}

	void preload(
		/*
		assuming this scene is registered w/ SceneManager,

		append load_assets() to a jobqueue with medium priority
		// scene_manager.load_assets(&this);
		from a developer perspective, if I want to preload
		the edges of this scene before a transition
		do {
			get_scene("***string alias***").preload()
		}
		*/
	);
	void load_assets();
	bool register_asset_to_scene(const std::string path);
	void load_assets_callback(/*
		callback for load_assets()
		assets_loaded = true;
		ready_to_transition = true;
	*/);
	void get_scene(/*
		visibility abstracts to the SceneManager
	*/);
	bool can_transition(/*
		return ready_to_transition && assets_loaded;
	*/);

	bool ready_to_transition; //
	bool all_assets_loaded;

private:
	// SceneManager *scene_manager;
	// HashMap assets;
	// uint32 key; // for this, assigned on creation
	// string key_alias // for human-readable
}