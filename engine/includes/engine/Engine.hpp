#include "./game/Game.hpp"
#include "./render/RenderMgr.hpp"

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

private:
	Game                    *game;
	RenderMgr               *renderer;
//	queue<>           *destroy;
};