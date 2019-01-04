#include "../includes/engine/Engine.hpp"

int main(int argc, char **argv) {
	Engine *engine;
	const char *title;
	const char *name;

	if (argc == 3) {
		title = argv[0];
		name = argv[1];
	} else {
		title = "Testing";
		name = "Test Window";
	}

	printf("%s\n%s\n", title, name);

	engine = new Engine();
	if (engine->_Init(title, name) == FAILURE)
		return (EXIT_FAILURE);

	/*
	 * if we're testing render mgr
	 */

#ifdef UNIT_TEST_RENDERMGR
	if (engine->getRenderer()._Test() == FAILURE)
		return (EXIT_FAILURE);

#endif

	/*
	 * if we're running the engine
	 */

#ifdef RUN
	if (engine->_Run() == FAILURE)
		return (EXIT_FAILURE);
	if (engine->_Destroy() == FAILURE)
		return (EXIT_FAILURE);

#else
	//ignore

#endif
	return (EXIT_SUCCESS);
}
