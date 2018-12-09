#include "../../topo.hpp"

class Game {
public:
	Game() {}
	~Game() {}
	Game(const Game &rhs) {*this = rhs;}
	Game& operator=(const Game &rhs) {
		*this = rhs; return (*this);
	}

	bool    _Init();
	bool    _Run();
	bool    _Destroy();

private:
	s_status    *status;
};