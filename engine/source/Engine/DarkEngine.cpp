#include "../../includes/Engine/DarkEngine.hpp"

void DarkEngine::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<DarkEngine*>(glfwGetWindowUserPointer(window));
	(void)width;
	(void)height;
	app->set_frame_buffer_resized(true);
}

std::vector<Object> DarkEngine::get_current_scene_objects() {
	/*
	chance for frustum culling
	if (options.optimizations.frustum_culling_enabled) {
		return scene_manager.get_current_scene().get_objects_in_view();
	} else {
		...
	}
	*/
	return scene_manager->get_current_test_scene()->get_all_objects();
}


bool DarkEngine::init_window(const char *title) {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	printf("creating draw window\n");
	_window = glfwCreateWindow(
		getScreenWidth(), getScreenHeight(),
		/*
		full screen or windowed ops
		user configurable
		*/
		title, nullptr/*glfwGetPrimaryMonitor()*/, nullptr);
	glfwSetWindowUserPointer(_window, this);
	glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
	return (!!_window);
}

bool DarkEngine::getScreenRes() {
//	// Get a handle to the desktop window
//	const HWND hDesktop;
//
//	hDesktop = GetDesktopWindow();
//	// Get the size of screen to the variable desktop
//	GetWindowRect(hDesktop, &desktop);
//	// The top left corner will have coordinates (0,0)
//	// and the bottom right corner will have coordinates
//	// (horizontal, vertical)
	_display = XOpenDisplay(NULL);
	_screen = DefaultScreenOfDisplay(_display);
	return (true);
}

bool DarkEngine::_Init(const char *title, const char *name) {
	Env = new Environ();
	if (Env->_Init() == FAILURE) {
		std::cout << "Wow, something's really messed up" << std::endl;
		return FAILURE;
	}

	scene_manager = new SceneManager();
	scene_manager->create_test_scene();

	if (getScreenRes() == FAILURE)
		return (FAILURE);
	if (init_window(name) == FAILURE)
		return (FAILURE);

	render_manager = new RenderManager(this);
	if (render_manager->_Init(title, name) == FAILURE) {
		return (Env->_Error(true, -1, __FILE__, __func__, __LINE__,  WHICH(renderer), "renderer init failed"));
	}

	return (SUCCESS);
}

bool DarkEngine::_Run() {
	// float delta;
	while (!glfwWindowShouldClose(_window)) {
		/*
		get delta elapsed, if not > enough,
		don't run physics this iter
		*/
		glfwPollEvents();
		// event_manager.poll_events(); // 
		render_manager->draw_frame();
	}
	render_manager->device_wait_idle();
	return (SUCCESS);
}

bool DarkEngine::_Destroy() {
	std::string errmsg = "destruction failed";

	// if (game->_Destroy() == FAILURE)
	// 	return (Env->_Error(true, -1, __FILE__, __func__, __LINE__,  WHICH(game), errmsg));
	if (render_manager->_Destroy() == FAILURE)
		return (Env->_Error(true, -1, __FILE__, __func__, __LINE__,  WHICH(renderer), errmsg));

	glfwDestroyWindow(_window);
	glfwTerminate();

	return (SUCCESS);
}