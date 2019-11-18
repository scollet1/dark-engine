#ifndef SCENE_HPP
#define SCENE_HPP

#include "../dark.hpp"
#include "../Assets/Asset.hpp"
#include "../Assets/Object.hpp"

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
	bool load();
	bool load_assets();
	bool register_asset_to_scene(const std::string path);
	bool mark_all_assets_loaded();
	bool mark_asset_loaded();
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
	bool transitioned();
	bool asset_loaded(Asset *asset);
	std::vector<Object> get_all_objects() {return objects;}

	bool ready_to_transition; //
	bool all_assets_loaded;

	void add_test_object_to_scene(Object object);

private:
	/*
	to hash the objects:
		create a map of maps for each Asset
		instance type(Object, Audio, etc...)

		hash each UID of each asset into their
		respective map with an index into their
		respective Asset vectors

		create a vector for each Asset and record
		the index

	when you want a specific Asset by UID:
		index into the map of Asset types

		index into the map holding the UIDs
		of that type

		index into the vector using the
		result of the UID map

		AssetList[AssetMap["Asset"][UID]]

		probably doesn't need to be so
		complicated
	*/
	// SceneManager *scene_manager;
	// HashMap assets;
	// uint32 key; // for this, assigned on creation
	// string key_alias // for human-readable
	std::vector<Object> objects;
};

#endif // SCENE_HPP