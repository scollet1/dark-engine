#include "../../includes/topo.hpp"

Engine::Engine() {}
Engine::~Engine() {}

bool    Engine::_Init() {
	t_task t;

	renderer = new Renderer();
	if (renderer._Init() == FAILURE)
		return (gEnv._Error(true, -1, __func__, WHICH(renderer), "renderer init failed"));

	t.func = renderer._Destroy();
	destroy.push(t);

	game = new Game();
	if (game._Init() == FAILURE)
		return (gEnv._Error(true, -1, __func__, WHICH(game), "game init failed"));

	t.func = game._Destroy();
	destroy.push(t);

	return (SUCCESS);
}

bool    Engine::_Run() {
	return (game._Run());
}

bool    Engine::_Destroy() {
	t_task t;

	while (!destroy.empty()) {
		if (!t = destroy.pop())
			return (gEnv._Error(true, -1, __func__, WHICH(t), "null variable"));
		if (t.func() == FAILURE)
			return (gEnv._Error(true, -1, __func__, WHICH(t.func), "failure destroying"));
	}
	return (SUCCESS);
}