#include "window.hpp"

#include <glad/glad.h>

#include <iostream>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

Window* Window::instance;
bool Window::instance_provided = false;

Window::Window(const int width, const int height, const ImVec4 background_color)
	: width(width), height(height), background_color(background_color) {
	if (!instance_provided) {
		instance = this;
		instance_provided = true;
	}
	else {
		throw std::runtime_error("Multi-Window support is not available for this application!");
	}
}

void Window::glfw_framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Window::glfw_error_callback(const int error, const char* description) {
	std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}


static void glfw_error_callback_invoker(int error, const char* description) {
	Window::get_instance().glfw_error_callback(error, description);
}

static void glfw_framebuffer_resize_callback_invoker(GLFWwindow* window, int width, int height) {
	Window::get_instance().glfw_framebuffer_resize_callback(window, width, height);
}

bool Window::setup(const GLFWkeyfun& key_callback) {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW3" << std::endl;
		return false;
	}

	glfwSetErrorCallback(glfw_error_callback_invoker);

	handle = glfwCreateWindow(width, height, "Texture Packer", nullptr, nullptr);
	if (!handle) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		return false;
	}

	glfwSetFramebufferSizeCallback(handle, glfw_framebuffer_resize_callback_invoker);

	recalculate_projection();

	glfwMakeContextCurrent(handle);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetKeyCallback(handle, key_callback);

	// Setup Dear ImGui context
	ImGui::CreateContext();
	imgui_io = &ImGui::GetIO();
	imgui_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Game pad Controls
	imgui_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	imgui_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

	ImGui_ImplGlfw_InitForOpenGL(handle, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	return true;
}

void Window::start_loop(const std::function<void()>& render) {
	while (!glfwWindowShouldClose(handle)) {
		glfwPollEvents();

		if (glfwGetWindowAttrib(handle, GLFW_ICONIFIED) != 0) {
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Clear to background colour
		glfwGetFramebufferSize(handle, &viewport_width, &viewport_height);
		glViewport(0, 0, viewport_width, viewport_height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(background_color.x * background_color.w,
		             background_color.y * background_color.w,
		             background_color.z * background_color.w,
		             background_color.w);

		render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (imgui_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(handle);
	}
}

void Window::clean() const {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(handle);
}

void Window::recalculate_projection() {
	float left = 0;
	float right = (float)width;
	float bottom = (float)height;
	float top = 0;

	const glm::mat4 orthographicMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	// glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), {currentSettings.zoom, currentSettings.zoom, currentSettings.zoom});
	// glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), {1,1,1});

	projection = orthographicMatrix;
}

Window& Window::get_instance() {
	return *instance;
}
