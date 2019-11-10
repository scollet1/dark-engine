#include "../includes/dark.h"
#include "../includes/Engine/DarkEngine.hpp"

int main(int argc, char **argv) {
	DarkEngine dark_engine;
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

	dark_engine = new DarkEngine();
	if (dark_engine._Init(title, name) == FAILURE)
		return (EXIT_FAILURE);

	/*
	 * if we're testing render mgr
	 */

#ifdef UNIT_TEST_RENDERMGR
	if (dark_engine.getRenderer()._Test() == FAILURE)
		return (EXIT_FAILURE);
#endif

	/*
	 * if we're running the dark_engine
	 */

#ifdef RUN
	if (dark_engine._Run() == FAILURE)
		return (EXIT_FAILURE);
	if (dark_engine._Destroy() == FAILURE)
		return (EXIT_FAILURE);

#else
	//ignore

#endif
	return (EXIT_SUCCESS);
}
