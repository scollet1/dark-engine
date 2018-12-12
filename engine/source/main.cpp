#include "../includes/engine/Engine.hpp"

int main(int argc, char ** argv) {
	Engine *engine;

	(void)argc;
	(void)argv;

	engine = new Engine();
	if (engine->_Init() == FAILURE)
		return (EXIT_FAILURE);

	g_job_q = new JobQueue();
	if (g_job_q->_Init(engine->getTpool()->getNumThreads()) == FAILURE)
		return (engine->getEnv()->_Error(true, -1, __func__, WHICH(g_job_q), "job queue init failed"));

	if (engine->_Run() == FAILURE)
		return (EXIT_FAILURE);
	if (engine->_Destroy() == FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}