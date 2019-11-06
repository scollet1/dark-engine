//#include "../../includes/topo.hpp"
#include "../../includes/engine/Engine.hpp"

bool    Engine::_Init(const char *title, const char *name) {

	Env = new Environ();
	if (Env->_Init() == FAILURE) {
		printf("Wow, something's really messed up.\n");
		return (FAILURE);
	}

	renderMgr = new RenderMgr();
	if (renderMgr->_Init(title, name) == FAILURE)
		return (Env->_Error(true, -1, __FILE__, __func__, __LINE__,  WHICH(renderer), "renderer init failed"));

	scene_manager = new SceneManager();
	if (scene_manager->_Init() == FAILURE)
		return (Env->_Error(true, -1, __FILE__, __func__, __LINE__,  WHICH(scene_manager), "scene_manager init failed"));

	return (SUCCESS);
}

bool    Engine::_Run() {
	return (game->_Run());
}

bool    Engine::_Destroy() {
	std::string errmsg = "destruction failed";

	if (renderMgr->_Destroy() == FAILURE)
		return (Env->_Error(true, -1, __FILE__, __func__, __LINE__,  WHICH(renderer), errmsg));
	
	return (SUCCESS);
}