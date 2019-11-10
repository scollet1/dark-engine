#include "../../../includes/engine/game/Game.hpp"

bool    Game::_Run() {
	while (status->running) {
		// do stuff
	}
	return (SUCCESS);
}

bool    Game::_Init() {
	if (!(status = (t_status*)memalloc(sizeof(t_status))))
		return (FAILURE);
	status->error = *(t_error*)memalloc(sizeof(t_error));
	status->running = true;
	return (SUCCESS);
}

bool    Game::_Destroy() {
	return (SUCCESS);
}