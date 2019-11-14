#include "../../includes/Scene/Scene.hpp"

void Scene::load_assets() {
	// change to hash map iterator
	// std::vector<std::string>::iterator i;
	// for (i = assets.begin(); i != assets.end(); i++) {
	// 	// hand it a callback
	// 	asset_manager.load_asset(i, asset_loaded);
	// }
}

bool Scene::transitioned() {
	// for asset in assets {
		
	// }
}

bool Scene::asset_loaded(Asset *asset) {
	// if asset.path in assets:
	// 	if assets.get(asset.path) == false:
	// 		return false;
	// 	else:
	// 		if (mark_asset_loaded() == FAILURE) {
	// 			return false;
	// 		}
	// 	return true
	// return false
}

bool Scene::mark_all_assets_loaded() {
	// count of objs in hash == total ticked count
	// uint32 count;
	// count = assets.count();

	// for (i in assets) {
	// 	if assets[i].loaded == true:
	// 		count--;
	// 	else:
	// 		break
	// }

	// all_assets_loaded = (count == 0);
	// return all_assets_loaded;	
}

bool Scene::mark_asset_loaded() {
	// wait for mutex {
	// 	if mark_all_assets_loaded() {
	// 		return FAILURE;
	// 	}
	// 	return SUCCESS;
	// }
	// return FAILURE;
}

bool register_asset_to_scene(const std::string path) {
	/*
	if not asset.path in assets:
		assets.insert(path);
		return SUCCESS;
	*/
	return FAILURE;
}

// ALIASES

void add_asset(const std::string path) {
	register_asset_to_scene(path); // alias
}

void Scene::load() {
	load_assets(); // alias
}

void Scene::add_test_object_to_scene(Object object) {
	objects.push_back(object);
}