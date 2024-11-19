#include "window.hpp"

#include <Input.hpp>
#include <glad/glad.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <iostream>

Window* Window::instance;
bool Window::instance_provided = false;

void glfw_error_callback(const int error, const char* description) {
	std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

Window::Window(const int width, const int height, const ImVec4 background_color)
	: width(width), height(height), background_color(background_color) {
	if (!instance_provided) {
		instance = this;
		instance_provided = true;
	} else {
		throw std::runtime_error("Multi-Window support is not available for this application!");
	}
}

void Window::glfw_framebuffer_resize_callback(GLFWwindow* window, const int width, const int height) {
	this->width = width;
	this->height = height;
	glViewport(0, 0, width, height);
	recalculate_projection();
}

static void glfw_framebuffer_resize_callback_invoker(GLFWwindow* window, const int width, const int height) {
	Window::get_instance().glfw_framebuffer_resize_callback(window, width, height);
}

bool Window::setup(const GLFWkeyfun& key_callback) {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW3" << std::endl;
		return false;
	}

	glfwSetErrorCallback(glfw_error_callback);

	this->handle = glfwCreateWindow(width, height, "Texture Packer", nullptr, nullptr);
	if (!this->handle) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		return false;
	}

	glfwSetFramebufferSizeCallback(this->handle, glfw_framebuffer_resize_callback_invoker);

	this->recalculate_projection();

	glfwMakeContextCurrent(this->handle);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetKeyCallback(this->handle, key_callback);

	Input::initialize(this);

	// Setup Dear ImGui context
	ImGui::CreateContext();
	this->imgui_io = &ImGui::GetIO();
	this->imgui_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	this->imgui_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	this->imgui_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

	ImGui_ImplGlfw_InitForOpenGL(this->handle, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	return true;
}

void Window::start_loop(const std::function<void()>& render) {
	while (!glfwWindowShouldClose(this->handle)) {
		glfwPollEvents();

		if (glfwGetWindowAttrib(this->handle, GLFW_ICONIFIED) != 0) {
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Clear to background colour
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(this->background_color.x * this->background_color.w,
		             this->background_color.y * this->background_color.w,
		             this->background_color.z * this->background_color.w,
		             this->background_color.w);

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
	glfwDestroyWindow(this->handle);
}

void Window::recalculate_projection() {
	const float left = 0;
	const float right = (float)this->width;
	const float bottom = (float)this->height;
	const float top = 0;

	const glm::mat4 orthographic_matrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	// glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), {currentSettings.zoom, currentSettings.zoom, currentSettings.zoom});
	// glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), {1,1,1});

	this->projection = orthographic_matrix;
}

Window& Window::get_instance() {
	return *instance;
}
