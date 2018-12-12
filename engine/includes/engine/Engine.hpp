#include "./game/Game.hpp"
#include "./render/RenderMgr.hpp"

#include "../threads/JobQueue.hpp"
#include "../threads/ThreadPool.hpp"

class Engine {
public:

	Engine() {};
	~Engine() {};
	Engine(const Engine &rhs) {*this = rhs;}
	Engine& operator=(const Engine &rhs) {
		*this = rhs; return (*this);
	}

	bool    _Init();
	bool    _Run();
	bool    _Destroy();

	Environ&    getEnv() {return (Env);}
	ThreadPool& getTpool() {return (tpool);}

private:
	ThreadPool              *tpool;
	RenderMgr               *renderer;
	Game                    *game;
	Environ                 *Env;
};