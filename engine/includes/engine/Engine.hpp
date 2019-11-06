#include "./game/Game.hpp"
#include "./render/RenderMgr.hpp"
#include "../threads/ThreadPool.hpp"

class Engine {
public:
	Engine() {};
	~Engine() {};
	Engine(const Engine &rhs) {*this = rhs;}
	Engine operator=(const Engine &rhs) {
		*this = rhs; return (*this);
	}

	bool    _Init(const char *title, const char *name);
	bool    _Run();
	bool    _Destroy();

	/*
	 * Getters
	 don't know aobut these being here :/
	 */
	Environ&    getEnv() {return (*Env);}
	ThreadPool& getTpool() {return (*thrpool);}
	RenderMgr&	getRenderer() {return (*renderMgr);}

private:
	ThreadPool              *thrpool;
	RenderMgr               *renderMgr;
	Game                    *game;
	Environ                 *Env;
	
	Display								*_display;
	Screen								*_screen;
	int									_width;
	int									_height;

	/* GLFW specs */
	GLFWwindow* 						_window;
	
    VkPhysicalDevice 					physicalDevice = VK_NULL_HANDLE;
    VkDevice 							_device;
};