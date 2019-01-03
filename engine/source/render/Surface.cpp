#include "../../includes/render/RenderMgr.hpp"

bool					RenderMgr::createSurface() {
	if (glfwCreateWindowSurface(_instance, _window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
	return (SUCCESS);
}
