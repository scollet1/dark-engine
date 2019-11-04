#include "./render/RenderMgr.hpp"
#include "../threads/ThreadPool.hpp"

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif

// #include <GLFW/glfw3.h>

#ifndef SCRNWOFFS
#define SCRNWOFFS -100
#endif

#ifndef SCRNHOFFS
#define SCRNHOFFS - 100
#endif

class DarkEngine {
public:
	DarkEngine() {};
	~DarkEngine() {};
	DarkEngine(const Engine &rhs) {*this = rhs;}
	DarkEngine operator=(const Engine &rhs) {
		*this = rhs; return (*this);
	}

	bool    _Init(const char *title, const char *name);
	bool    _Run(); // what loop is this
	bool    _Destroy();

	uint32_t getScreenWidth() {
			// return (800);}
		return (_screen->width + SCRNWOFFS);
	}
	uint32_t getScreenHeight() {
			// return (400);}
		return (_screen->height + SCRNHOFFS);
	}

	void setFrameBufferResized(bool stat) {
		framebufferResized = stat;
	}

	/*
	 * Getters
	 */
	Environ&    getEnv() {return (*Env);}
	ThreadPool& getTpool() {return (*thrpool);}
	RenderMgr&	getRenderer() {return (*renderMgr);}
	GLFWwindow *get_window() {return (*_window);}
	static void	framebufferResizeCallback(GLFWwindow* window, int width, int height);

private:
	ThreadPool              *thrpool;
	RenderMgr               *renderMgr;
	Game                    *game;
	Environ                 *Env;

	/* GLFW specs */
	GLFWwindow	 			*_window;
	Display								*_display;
	Screen								*_screen;
	int									_width;
	int									_height;
};