#include "../includes/topo.hpp"

bool    Topo::_Start() {
	gEnv = new Environ();
	if (gEnv._Init() == FAILURE)
		return (FAILURE);
	this.engine = new Engine();
	if (this.engine._Init() == FAILURE)
		return (gEnv._Error(true, -1, __func__, WHICH(this.engine), "starting Topo failed"));
	return (SUCCESS);
}

bool    Topo::_Run() {
	return (engine._Run());
}

bool    Topo::_Destroy() {
	return (this.engine._Destroy());
}