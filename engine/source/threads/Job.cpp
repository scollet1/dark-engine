#include "../../includes/threads/JobQueue.hpp"

bool    Job::run() {
	return (func(data));
}

bool    Job::setData(const &d) {
	data = d;
	return (true);
}

bool    Job::setFunc(const &f) {
	func = f;
	return (false);
}