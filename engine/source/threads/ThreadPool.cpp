#include "../../includes/threads/ThreadPool.hpp"

bool ThreadPool::_Init(unsigned short num_threads) {
	_hard_threads = std::thread::hardware_concurrency();

	if (num_threads > MAX_THREADS)
		num_threads = 4;
	// create threadpool
	num_threads = _hard_threads;
	_thread_pool = new std::thread[num_threads];
	return (true);
}

unsigned short ThreadPool::getNumThreads() {
	return (num_threads);
}

unsigned short ThreadPool::getMaxThreads() {
	return (_hard_threads * 32);
}

unsigned short ThreadPool::getHardThreads() {
	return (_hard_threads);
}