//#include "../../includes/topo.hpp"
#include "../../includes/engine/Engine.hpp"


void	DarkEngine::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<DarkEngine*>(glfwGetWindowUserPointer(window));
	(void)width;
	(void)height;
	app->setFrameBufferResized(true);
}


bool	DarkEngine::initWindow(const char *title) {
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
	return (!!dark_engine.get_window());
}

bool	RenderMgr::getScreenRes() {
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

bool    Engine::_Init(const char *title, const char *name) {
	Env = new Environ();
	if (Env->_Init() == FAILURE) {
		printf("Wow, something's really messed up.\n");
		return (FAILURE);
	}

	thrpool = new ThreadPool();
	if (thrpool->_Init(128) == FAILURE)
		return (Env->_Error(true, -1, __FILE__, __func__, __LINE__,  WHICH(thrpool), "thread pool init failed"));

	if (getScreenRes() == FAILURE)
		return (FAILURE);
	if (initWindow(name) == FAILURE)
		return (FAILURE);

	renderMgr = new RenderMgr();
	if (renderMgr->_Init(title, name) == FAILURE)
		return (Env->_Error(true, -1, __FILE__, __func__, __LINE__,  WHICH(renderer), "renderer init failed"));

	return (SUCCESS);
}

bool    Engine::_Run() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		// event_manager.poll_events(); // 
		render_manager.draw_frame();
	}
	vkDeviceWaitIdle(device);
	return (SUCCESS);
}

bool    Engine::_Destroy() {
	std::string errmsg = "destruction failed";

	if (game->_Destroy() == FAILURE)
		return (Env->_Error(true, -1, __FILE__, __func__, __LINE__,  WHICH(game), errmsg));
	if (renderMgr->_Destroy() == FAILURE)
		return (Env->_Error(true, -1, __FILE__, __func__, __LINE__,  WHICH(renderer), errmsg));
	return (SUCCESS);
}