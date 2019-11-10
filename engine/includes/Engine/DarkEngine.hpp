#ifndef DARKENGINE_H
#define DARKENGINE_H

#include "../dark.h"
#include "../Render/RenderManager.hpp"

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif

#include <GLFW/glfw3.h>
#include <X11/Xlib.h>

#ifndef SCRNWOFFS
#define SCRNWOFFS -100
#endif

#ifndef SCRNHOFFS
#define SCRNHOFFS -100
#endif

class DarkEngine {
public:
	DarkEngine() {
		set_frame_buffer_resized(false);
	}
	~DarkEngine() {};
	DarkEngine(const DarkEngine &rhs) {*this = rhs;}
	DarkEngine operator=(const DarkEngine &rhs) {
		*this = rhs;
		return *this;
	}

	bool    _Init(const char *title, const char *name);
	bool    _Run(); // what loop is this
	bool    _Destroy();

	uint32_t getScreenWidth() {
		return (_screen->width + SCRNWOFFS);
	}
	uint32_t getScreenHeight() {
		return (_screen->height + SCRNHOFFS);
	}

	void set_frame_buffer_resized(bool stat) {framebufferResized = stat;}
	bool get_frame_buffer_resized() {return framebufferResized;}

	/*
	 * Getters
	 */
	Environ&    getEnv() {return (*Env);}
	// ThreadPool& getTpool() {return (*thrpool);}
	RenderManager &get_render_manager() {return *render_manager;}
	GLFWwindow *get_window() {return (_window);}
	bool init_window(const char *title);
	static void	framebufferResizeCallback(GLFWwindow* window, int width, int height);

private:
	bool getScreenRes();

	// ThreadPool              *thrpool;
	RenderManager               *render_manager;
	// Game                    *game;
	Environ                 *Env;

	/* GLFW specs */
	GLFWwindow *_window;
	Display *_display;
	Screen *_screen;
	int	_width;
	int	_height;

	bool framebufferResized;
};

#endif // DARKENGINE_H