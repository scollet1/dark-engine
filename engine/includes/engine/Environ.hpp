class Environ {
public:

	Environ() {}
	~Environ() {}

	bool    _Init();
	bool    _Destroy();
	void    _Err_Log();
	bool    _Error(bool fatal, int errcode, const char *func, const char *var, const char *errmsg);

	t_status    *getStatus();


private:
	t_status    *status;
}