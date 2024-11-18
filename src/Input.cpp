#include "Input.hpp"

Window* Input::window;
glm::vec2 Input::mouse_position;

void Input::initialize(Window* window) {
	Input::window = window;

	glfwSetCursorPosCallback(window->handle, mouse_position_callback);
}

const glm::vec2& Input::get_mouse_position() {
	return Input::mouse_position;
}

bool Input::get_key_pressed(const int key) {
	return glfwGetKey(Input::window->handle, key) == GLFW_PRESS;
}

bool Input::get_mouse_button_pressed(const int button) {
	return glfwGetMouseButton(Input::window->handle, button) == GLFW_PRESS;
}

void Input::mouse_position_callback(GLFWwindow* window, const double x_position, const double y_position) {
	Input::mouse_position = glm::vec2(x_position, y_position);
}
