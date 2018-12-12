#include "../topo.hpp"
#include <thread>

class Thread {
public:
	Thread() {}
	~Thread() {}

	bool            _Init(unsigned int num_threads);

	bool            consume();

	unsigned int    getNumThreads();
	unsigned int    getMaxThreads();
	unsigned int    getHardThreads();

private:
	unsigned int                _hard_threads;
	unsigned int                num_threads;
	std::vector<std::thread>    _thread_pool;
	t_status                    *status;

};