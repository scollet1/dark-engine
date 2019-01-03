#include "../topo.hpp"
#include <thread>

#ifndef MAX_THREADS
#define MAX_THREADS 65000
#endif

typedef struct              s_task {
	unsigned int          	id;
	std::function<void()>   func;
}                           t_task;

class ThreadPool {
public:
	ThreadPool() {}
	~ThreadPool() {}

	bool            _Init(unsigned short num_threads);

	/*
	** Getters
	*/
	unsigned short    getNumThreads();
	unsigned short    getMaxThreads();
	unsigned short    getHardThreads();

private:
	unsigned short                		_hard_threads;
	unsigned short                		num_threads;
	std::thread					*_thread_pool;
	t_status                    *status;
};