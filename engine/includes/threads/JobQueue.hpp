#include <boost/lockfree/queue.hpp>
#include "./Job.hpp"

class JobQueue {
public:
	JobQueue() {};
	~JobQueue() {};
	JobQueue(const JobQueue &rhs) {*this = rhs;};
	JobQueue& operator=(const JobQueue &rhs) {
		*this = rhs; return (*this);
	}

	bool    _Init();
	bool    _Run();
	bool    _Destroy();

	/*
	** atomic boost push() wrapper
	** pushes job element onto buffer
	*/
	bool feed(Job &elem);
	/*
	** Block until job queue is exhausted
	*/
	bool digest();
	bool empty();
	Job* consume();

private:
	/*
	** Initialize a job queue of 10,000 elements, each with a
	** reference to Job instance
	*/
	// TODO : make dynamic
	boost::lockfree::queue
	        <std::atomic<*Job>, boost::lockfree::fixed_sized<false>> job_q;
};