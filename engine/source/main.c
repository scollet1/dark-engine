#include "../includes/dark.hpp"
#include "../includes/Engine/DarkEngine.hpp"

int main(int argc, char **argv) {
	DarkEngine *dark_engine;
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
	if (dark_engine->_Init(title, name) == FAILURE) {
		printf("Could not initialize dark engine\n");
		return (EXIT_FAILURE);
	}

	if (dark_engine->_Run() == FAILURE) {
		printf("Could not run dark engine\n");
		return (EXIT_FAILURE);
	}

	if (dark_engine->_Destroy() == FAILURE)
		return (EXIT_FAILURE);

	return (EXIT_SUCCESS);
}
