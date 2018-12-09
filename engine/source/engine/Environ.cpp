#include "../../includes/topo.hpp"

Environ::Environ() {}
Environ::~Environ() {}

bool        Environ::_Init() {
	if (!(status = (t_status*)memalloc(sizeof(t_status))))
		return (FAILURE);
	return (SUCCESS);
}

bool        Environ::_Destroy() {
	//
	return (true);
}

t_status    *Environ::getStatus() {
	return (status);
}

void        Environ::_Err_Else(int errcode, std::string func, std::string var, std::string errmsg) {
	std::string full_trace;
	t_trace     *trace;

	trace = (t_trace*)memalloc(sizeof(t_error));
	full_trace = std::string(NON_FATAL_ERR) + " in " + func + ": "
				+ var + " " + errmsg + " (ERR_ELSE)\n";
	trace->errmsg = full_trace;
	trace->out = true;
	trace->errcode = errcode;
	status->error.trace.push(*trace);
}

void        Environ::_Err_Proc(int errcode, std::string func, std::string var, std::string errmsg) {
	std::string full_trace;
	t_trace     *trace;

	trace = (t_trace*)memalloc(sizeof(t_error));
	full_trace = NON_FATAL_ERR + " in " + func + ": "
			+ var + " " + errmsg + " (ERR_PRC)\n";
	status->running = false;
	trace->errmsg = full_trace;
	trace->out = true;
	trace->errcode = errcode;
	status->error.trace.push(*trace);
}

void        Environ::_Err_Sys(int errcode, std::string func, std::string var, std::string errmsg) {
	std::string full_trace;
	t_trace     *trace;

	trace = (t_trace*)memalloc(sizeof(t_error));
	full_trace = FATAL_ERR + " in " + func + ": " \
				+ var + " " + errmsg + " (ERR_SYS)\n";
	status->running = false;
	trace->errmsg = full_trace;
	trace->out = true;
	trace->errcode = errcode;
	status->error.trace.push(*trace);
}

void        Environ::_Err_Log() {
	std::ofstream       file;
	t_trace             *result;

	file.open(status->error.log);
	while (!status->error.trace.empty()) {
		result = &status->error.trace.top();
		if (result->out)
			std::cout << result->errmsg << std::endl;
		file << result->errmsg << std::endl;
		status->error.trace.pop();
		free(result);
	}
	file.close();
}

bool        Environ::_Error(
			bool fatal, int errcode, std::string func, std::string var, std::string errmsg) {
	if (errcode == ERR_SYS)
		_Err_Sys(errcode, func, var, errmsg);
	else if (errcode == ERR_PROC)
		_Err_Proc(errcode, func, var, errmsg);
	else if (errcode == ERR_ELSE)
		_Err_Else(errcode, func, var, errmsg);
	_Err_Log();
	return (fatal);
}
