#include "../../includes/topo.hpp"

Environ::Environ() {}
Environ::~Environ() {}

bool        Environ::_Init() {
	if (!(this.status = (t_status*)memalloc(sizeof(t_status))))
		return (FAILURE);
	return (SUCCESS);
}

bool        Environ::_Destroy() {
	//
}

t_status    *Environ::getStatus() {
	return (this.status);
}

void        Environ::_Err_Log() {
	ofstream    file;
	t_trace     result;

	file.open(this.status->error.log);
	while (!this.status->error.trace.empty()) {
		result = this.status->error.trace.pop();
		if (result.out)
			std::cout << result.errmsg << std::endl;
		std::file << result.errmsg << std::endl;
	}
	file.close();
}

bool        Environ::_Error(
			bool fatal, int errcode, const char *func, const char *var, const char *errmsg) {
	if (errcode == ERR_SYS)
		_Err_Sys(errcode, func, var, errmsg);
	else if (errcode == ERR_PROC)
		_Err_Proc(errcode, func, var, errmsg);
	else if (errcode == ERR_ELSE)
		_Err_Else(errocde, func, var, errmsg));
	_Err_Log();
	return (fatal);
}
