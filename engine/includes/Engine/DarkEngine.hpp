#ifndef DARKENGINE_H
#define DARKENGINE_H

#include <X11/Xlib.h>

#include "../dark.hpp"
#include "./Environ.hpp"
#include "../Scene/SceneManager.hpp"
#include "../Render/RenderManager.hpp"

#ifndef SCRNWOFFS
#define SCRNWOFFS -100
#endif

#ifndef SCRNHOFFS
#define SCRNHOFFS -100
#endif

class RenderManager;

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
		return 800;//(screen->width + SCRNWOFFS);
	}
	uint32_t getScreenHeight() {
		return 600;//(screen->height + SCRNHOFFS);
	}

	void set_frame_buffer_resized(bool stat) {framebufferResized = stat;}
	bool get_frame_buffer_resized() {return framebufferResized;}

	/*
	 * Getters
	 */
	std::vector<Object> get_current_scene_objects();
	Environ&    getEnv() {return (*Env);}
	// ThreadPool& getTpool() {return (*thrpool);}
	RenderManager &get_render_manager() {return *render_manager;}
	GLFWwindow *get_window() {return (_window);}
	bool init_window(const char *title);
	static void	framebufferResizeCallback(GLFWwindow* window, int width, int height);

private:
	bool init_glfw();

	// ThreadPool              *thrpool;
	RenderManager *render_manager;
	SceneManager *scene_manager;
	// Game                    *game;
	Environ                 *Env;

	/* GLFW specs */
	GLFWwindow *_window;
	GLFWmonitor *monitor;
	GLFWvidmode *screen;
	int	_width;
	int	_height;
	bool framebufferResized;
};

#endif // DARKENGINE_H