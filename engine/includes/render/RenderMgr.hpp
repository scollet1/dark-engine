#include "../topo.hpp"

class RenderMgr {
public:
	RenderMgr() {};
	~RenderMgr() {};
	RenderMgr(const RenderMgr &rhs) {*this = rhs;};
	RenderMgr& operator=(const RenderMgr &rhs) {
		*this = rhs; return (*this);
	}

	bool    _Init();
	bool    _Run();
	bool    _Destroy();

private:

};