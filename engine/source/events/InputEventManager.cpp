void InputEventManager::set_mouse_button_callbacK(f) {
	glfwSetMouseButtonCallback(this.engine.window, on_mouse_press);
	/*
	hash user-supplied function pointer to the intended mouse keycode
	*/
}

void InputEventManager::on_mouse_press(GLFWwindow* window, int button, int action, int mods) {
	/*
	capture function pointer and call that here with specifier flags
	should we allow users to specify a separate function for key_hold/press?
	or should we just call this with a flag and trigger an event when key_press terminates?
	*/
}