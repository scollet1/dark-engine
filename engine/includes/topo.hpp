#ifndef TOPO_HPP
#define TOPO_HPP

#include "./defined.hpp"

#include "./engine/Engine.hpp"
#include "./engine/Environ.hpp"
#include "./engine/game/Game.hpp"

#include "./utils/utils.hpp"

#ifndef WHICH
#define WHICH (#var)
#endif

Environ gEnv;

typedef struct              s_task {
	int                     id;
	std::function<void()>   func;
}                           t_task;

typedef struct  s_trace {
	int         errcode;
	char        *errmsg;
	bool        out;
}               t_trace;

typedef struct  s_error {
	int         fd;
	char        *log;
	stack       <t_trace> trace;
}               t_error;

typedef struct  s_status {
	bool        running;
	t_error     error;
}               t_status;


class Topo {
public:
	Topo() {}
	~Topo() {}

	_Start();
	_Run();
	_Destroy();

private:
	Engine  engine;
};

#endif