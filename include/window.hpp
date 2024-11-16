#pragma once

#define GLFW_INCLUDE_NONE
#include <functional>
#include <GLFW/glfw3.h>

#include <imgui.h>

struct window {
	GLFWwindow* handle = nullptr;
	int width, height;
	ImGuiIO* imgui_io = nullptr;
	int viewport_width{0}, viewport_height{0};
	ImVec4 background_color;

	window(int width, int height, ImVec4 background_color);
};

void glfw_error_callback(int error, const char* description);
bool window_setup(window* window,const GLFWkeyfun& key_callback);
void window_start_loop(window& window, const std::function<void()>& render);
void clean_window(const window& window);
