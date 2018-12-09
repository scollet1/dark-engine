//#include "../../includes/topo.hpp"
#include "../../includes/engine/Engine.hpp"

Environ                     *gEnv;

bool    Engine::_Init() {
	renderer = new RenderMgr();
	if (renderer->_Init() == FAILURE)
		return (gEnv->_Error(true, -1, __func__, WHICH(renderer), "renderer init failed"));
	game = new Game();
	if (game->_Init() == FAILURE)
		return (gEnv->_Error(true, -1, __func__, WHICH(game), "game init failed"));
	return (SUCCESS);
}

bool    Engine::_Run() {
	return (game->_Run());
}

bool    Engine::_Destroy() {
	std::string errmsg = "destruction failed";

	if (game->_Destroy() == FAILURE)
		return (gEnv->_Error(true, -1, __func__, WHICH(game), errmsg));
	delete game;
	if (renderer->_Destroy() == FAILURE)
		return (gEnv->_Error(true, -1, __func__, WHICH(renderer), errmsg));
	delete renderer;
	return (SUCCESS);
}