#include "../includes/engine/Engine.hpp"

int main(int argc, char ** argv) {
	Engine *engine;

	(void)argc;
	(void)argv;
	engine = new Engine();
	if (engine->_Init() == FAILURE)
		return (EXIT_FAILURE);
	if (engine->_Run() == FAILURE)
		return (EXIT_FAILURE);
	if (engine->_Destroy() == FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}