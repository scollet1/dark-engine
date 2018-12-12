#include "../../includes/threads/JobQueue.hpp"

bool        JobQueue::feed(void*(*f)(*void), void *data) {
	Job *job;

	job = new Job();
	job->setFunc(&f);
	job->setData(&data);
	while (!job_q.push(job));
	return (true);
}

Job*        JobQueue::consume() {
	Job *job;

	while (true) {
		if ((job = job_q.pop()))
			return (job);
	}
}