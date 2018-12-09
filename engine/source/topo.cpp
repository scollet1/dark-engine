#include "../includes/topo.hpp"

bool            Topo::_Start() {
	Engine      *engine;
	gEnv = new Environ();
	if (gEnv->_Init() == FAILURE)
		return (FAILURE);
	engine = new Engine();
	if (engine->_Init() == FAILURE)
		return (gEnv->_Error(true, -1, __func__, WHICH(engine), "starting Topo failed"));
	return (SUCCESS);
}

bool    Topo::_Run() {
	return (engine._Run());
}

bool    Topo::_Destroy() {
	return (engine._Destroy());
}