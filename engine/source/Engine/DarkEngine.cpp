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
	return scene_manager->get_current_scene().get_all_objects();
}

bool DarkEngine::init_window(const char *title) {
	if (!(_window = glfwCreateWindow(
		getScreenWidth(), getScreenHeight(),
		/*
		full screen or windowed ops
		user configurable
		*/
		title, nullptr/*glfwGetPrimaryMonitor()*/, nullptr))) {
		return FAILURE;
	}
	glfwSetWindowUserPointer(_window, this);
	glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
	return (!!_window);
}

bool DarkEngine::init_glfw() {
	if (!glfwInit()) return FAILURE;
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	if (!(monitor = glfwGetPrimaryMonitor())) return FAILURE;
	if (!(screen = (GLFWvidmode*)glfwGetVideoMode(monitor))) return FAILURE;
	return SUCCESS;
}

bool DarkEngine::_Init(const char *title, const char *name) {
	Env = new Environ();
	if (Env->_Init() == FAILURE) {
		std::cout << "Wow, something's really messed up" << std::endl;
		return FAILURE;
	}

	if (init_glfw() == FAILURE) return FAILURE;
	if (init_window(name) == FAILURE) return FAILURE;

	scene_manager = new SceneManager();
	scene_manager->load_scene_graph("\
		string does not matter for testing\
	");

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

	if (render_manager->_Destroy() == FAILURE) {
		return (Env->_Error(true, -1, __FILE__, __func__, __LINE__,  WHICH(renderer), errmsg));
	}

	glfwDestroyWindow(_window);
	glfwTerminate();

	return (SUCCESS);
}