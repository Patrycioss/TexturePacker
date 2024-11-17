#include "app.hpp"

App* App::instance;

static void glfw_key_callback_invoker(GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
	App::get_instance()->glfw_key_callback(window, key, scancode, action, mods);
}

App::App(int width, int height, const ImVec4& background_color)
	: window(width, height, background_color) {
	if (instance != nullptr) {
		throw std::runtime_error("Cannot create a second App instance!");
	}

	instance = this;
}

App::~App() {
	NFD_Quit();
	window.clean();
	glfwTerminate();
}

void App::start() {
	if (!window.setup(glfw_key_callback_invoker)) {
		std::cerr << "Failed to setup window" << std::endl;
		throw std::runtime_error("Failed to setup window!");
	}

	NFD_Init();

	window.start_loop(
		[&]() {
			// Rendering
			for (const DisplayImage& display_image : display_images) {
				display_image.draw(window);
			}

			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Open", "Ctrl+O")) {
						trigger_image_load_dialogue();
					}
					if (ImGui::MenuItem("Save", "Ctrl+S")) {
					}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
			ImGui::Render();
		});
}

void App::glfw_key_callback(GLFWwindow* window, const int key, int scancode, const int action, const int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		return;
	}

	if (key == GLFW_KEY_O && (mods & (GLFW_MOD_CONTROL)) && action == GLFW_PRESS) {
		trigger_image_load_dialogue();
		return;
	}

	if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
		if (!display_images.empty()) {
			display_images.pop_back();
		}
		return;
	}
}

App* App::get_instance() {
	return instance;
}

void App::trigger_image_load_dialogue() {
	const std::string path = choose_file_dialogue(image_open_args);

	if (path.empty()) {
		std::cerr << "No file selected?" << std::endl;
		return;
	}

	Image image{};

	if (image.load(path)) {
		display_images.reserve(display_images.size() + 1);
		DisplayImage& display_image = display_images.emplace_back(image);

		if (!display_image.create()) {
			std::cerr << "Failed to create display image" << std::endl;
			display_images.pop_back();
		}
	};
}
