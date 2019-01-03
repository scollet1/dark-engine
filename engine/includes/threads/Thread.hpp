#include "../topo.hpp"
#include <thread>
#include "../threads/JobQueue.hpp"

class Thread {
public:
	Thread() {}
	~Thread() {}

	bool            _Init(unsigned int num_threads);
	bool            consume();

private:
	std::vector<std::thread>	_fibers;
	JobQueue					j_q;

};