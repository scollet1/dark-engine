#include "../topo.hpp"
#include <boost/lockfree/queue>

class JobQueue {
public:
	JobQueue() {};
	~JobQueue() {};
	JobQueue(const JobQueue &rhs) {*this = rhs;};
	JobQueue& operator=(const JobQueue &rhs) {
		*this = rhs; return (*this);
	}

	/*
	** atomic boost push() wrapper
	** pushes job element onto buffer
	*/
	bool _jq_push(Job &elem);
	Job& _jq_consume();

private:
	/*
	** Initialize a job queue of 10,000 elements, each with a
	** reference to Job instance
	*/
	// TODO : make dynamic
	boost::lockfree::queue
	        <std::atomic<*Job>, fixed_sized<true>> job_q {10000};
};