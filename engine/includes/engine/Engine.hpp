#include "./game/Game.hpp"
#include "./render/RenderMgr.hpp"
#include "../threads/ThreadPool.hpp"

class Engine {
public:
	Engine() {};
	~Engine() {};
	Engine(const Engine &rhs) {*this = rhs;}
	Engine operator=(const Engine &rhs) {
		*this = rhs; return (*this);
	}

	bool    _Init(const char *title, const char *name);
	bool    _Run();
	bool    _Destroy();

	/*
	 * Getters
	 */
	Environ&    getEnv() {return (*Env);}
	ThreadPool& getTpool() {return (*thrpool);}
	RenderMgr&	getRenderer() {return (*renderMgr);}

private:
	ThreadPool              *thrpool;
	RenderMgr               *renderMgr;
	Game                    *game;
	Environ                 *Env;
};