#pragma once

#define GLFW_INCLUDE_NONE
#include <functional>
#include <GLFW/glfw3.h>

#include <imgui.h>

#include "glm/mat4x4.hpp"

struct window {
	GLFWwindow* handle = nullptr;
	int width, height;
	ImGuiIO* imgui_io = nullptr;
	int viewport_width{0}, viewport_height{0};
	ImVec4 background_color;
	glm::mat4x4 projection{};

	window(int width, int height, ImVec4 background_color);
};

void framebuffer_resize_callback(GLFWwindow* window, int width, int height);
void glfw_error_callback(int error, const char* description);
bool window_setup(window* window,const GLFWkeyfun& key_callback);
void window_start_loop(window& window, const std::function<void()>& render);
void clean_window(const window& window);
void window_recalculate_projection(window& window);
