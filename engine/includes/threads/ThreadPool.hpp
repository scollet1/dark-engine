#include "../topo.hpp"
#include <thread>

typedef struct              s_task {
	int                     id;
	std::function<void()>   func;
}                           t_task;

class ThreadPool {
public:
	ThreadPool() {}
	~ThreadPool() {}

	bool    _Init(unsigned int num_threads);

	/*
	** atomic boost push() wrapper
	** pushes job element onto buffer
	*/

	unsigned int getNumThreads();
	unsigned int getMaxThreads();
	unsigned int getHardThreads();

private:
	unsigned int                _hard_threads;
	unsigned int                num_threads;
	std::vector<std::thread>    _thread_pool;
};