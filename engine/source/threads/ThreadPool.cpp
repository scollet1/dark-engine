#include "../../includes/threads/ThreadPool.hpp"

bool ThreadPool::_Init(unsigned int num_threads) {
	_hard_threads = std::thread::hardware_concurrency();

	if (num_threads > _max_Threads)
		num_threads = 4;
	// create threadpool
	num_threads = _hard_threads * 16;
	std::vector<std::thread> tpool (num_threads, consume);
	_thread_pool = tpool;
}

unsigned int ThreadPool::getNumThreads() {
	return (num_threads);
}

unsigned int ThreadPool::getMaxThreads() {
	return (_hard_threads * 32)
}

unsigned int ThreadPool::getHardThreads() {
	return (_hard_threads);
}