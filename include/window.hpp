#pragma once

#define GLFW_INCLUDE_NONE
#include <functional>
#include <GLFW/glfw3.h>

#include <imgui.h>

#include "glm/mat4x4.hpp"

class Window {
	public:
		GLFWwindow* handle = nullptr;
		int width, height;
		ImGuiIO* imgui_io = nullptr;
		int viewport_width{0}, viewport_height{0};
		ImVec4 background_color;
		glm::mat4x4 projection{};

		Window(int width, int height, ImVec4 background_color);

	private:
		static Window* instance;
		static bool instance_provided;

	public:
		void glfw_framebuffer_resize_callback(GLFWwindow* window, int width, int height);
		void glfw_error_callback(int error, const char* description);

		bool setup(const GLFWkeyfun& key_callback);
		void start_loop(const std::function<void()>& render);
		void clean() const;
		void recalculate_projection();

		static Window& get_instance();

	private:
};
