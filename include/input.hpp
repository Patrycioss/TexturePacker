#pragma once

#include <window.hpp>
#include <glm/vec2.hpp>

class Input {
	public:

	private:
		static Window* window;
		static glm::vec2 mouse_position;

	public:
		static void initialize(Window* window);
	
		static const glm::vec2& get_mouse_position();
		[[nodiscard]] static bool get_key_pressed(int key);
		[[nodiscard]] static bool get_mouse_button_pressed(int button);

	private:
		Input() = delete;
		Input(const Input&) = delete;
		Input(const Input&&) = delete;
		~Input() = delete;

		static void mouse_position_callback(GLFWwindow* window, double x_position, double y_position);
};
