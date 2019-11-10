#ifndef ENVIRON_HPP
#define ENVIRON_HPP

typedef struct              s_trace {
	int                     errcode;
	std::string             errmsg;
	bool                    out;
}                           t_trace;

typedef struct              s_error {
	int                     fd;
	char                    *log;
	std::stack              <t_trace> trace;
}                           t_error;

typedef struct              s_status {
	bool                    running;
	t_error                 error;
}                           t_status;

class Environ {
public:

	Environ();
	~Environ();
	Environ(const Environ &rhs) {*this = rhs;}
	Environ& operator=(const Environ &rhs) {
		*this = rhs; return (*this);
	}

	t_status    *getStatus();

	bool        _Init();
	bool        _Destroy();
	bool        _Error(bool fatal, int errcode,
						std::string file,
						std::string func,
						int line,
						std::string var,
						std::string errmsg);

	void        _Err_Log();
	void        _Err_Sys(int errcode,
						 std::string file,
						 std::string func,
						 std::string line,
						 std::string var,
						 std::string errmsg);
	void        _Err_Proc(int errcode,
						  std::string file,
						  std::string func,
						  std::string line,
						  std::string var,
						  std::string errmsg);
	void        _Err_Else(int errcode,
						  std::string file,
						  std::string func,
						  std::string line,
						  std::string var,
						  std::string errmsg);

private:
	t_status    *status;
};

#endif