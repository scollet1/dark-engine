#include "../types.hpp"

class Job {
public:
	Job() {};
	~Job() {};
	Job(const Job &rhs) {*this = rhs;};
	Job& operator=(const Job &rhs) {
		*this = rhs; return (*this);
	}

	bool    run();

	/*
	** Getters
	*/

	/*
	** Setters
	*/
	bool    setFunc(const std::function<bool(void*)> &f);
	bool    setData(const void* &d);


private:
	int                         id;
	std::string                 *handle;
	void                        *data;
	std::function<bool(void*)>   func;
	t_status                    *status;

};