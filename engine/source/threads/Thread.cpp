#include "../../includes/threads/Thread.hpp"

bool        Thread::consume() {
	Job *job;

	while (status->running) {
		if (!g_job_q->empty()) {
			if (!(job = g_job_q->consume()))
				continue;
			if (job->run() == FAILURE)
				return (false); // TODO : ??
		}
	}
	return (true);
}