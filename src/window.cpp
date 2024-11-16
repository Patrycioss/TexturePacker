#include "window.hpp"

#include <glad/glad.h>

#include <iostream>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

window::window(const int width, const int height, const ImVec4 background_color)
	: width(width), height(height), background_color(background_color) {
}

void glfw_error_callback(const int error, const char* description) {
	std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

bool window_setup(window* window, const GLFWkeyfun& key_callback) {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW3" << std::endl;
		return false;
	}

	glfwSetErrorCallback(glfw_error_callback);

	window->handle = glfwCreateWindow(window->width, window->height, "Texture Packer", nullptr, nullptr);
	if (!window->handle) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		return false;
	}

	glfwMakeContextCurrent(window->handle);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetKeyCallback(window->handle, key_callback);

	// Setup Dear ImGui context
	ImGui::CreateContext();
	window->imgui_io = &ImGui::GetIO();
	window->imgui_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Game pad Controls
	window->imgui_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	window->imgui_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

	ImGui_ImplGlfw_InitForOpenGL(window->handle, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	return true;
}

void window_start_loop(window& window, const std::function<void()>& render) {
	while (!glfwWindowShouldClose(window.handle)) {
		glfwPollEvents();

		if (glfwGetWindowAttrib(window.handle, GLFW_ICONIFIED) != 0) {
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Clear to background colour
		glfwGetFramebufferSize(window.handle, &window.viewport_width, &window.viewport_height);
		glViewport(0, 0, window.viewport_width, window.viewport_height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(window.background_color.x * window.background_color.w,
		             window.background_color.y * window.background_color.w,
		             window.background_color.z * window.background_color.w,
		             window.background_color.w);

		render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (window.imgui_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(window.handle);
	}
}

void clean_window(const window& window) {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window.handle);
}
