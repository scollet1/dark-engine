class InputManager : EventManager {
private:
	/*
	privately-linked mouse callback. Link engine builtins to userspace logic to call
	on event. on_left_mouse_press(), on_left_mouse_hold(), etc...

	will user override these functions??
	will user be expected to bind these builtins to their own callbacks??

	only time will tell
	*/
	void set_mouse_button_callbacK();
	void on_mouse_press(GLFWwindow* window, int button, int action, int mods);

	DarkEngine	*dark_engine; // move to parent EventManager
}
