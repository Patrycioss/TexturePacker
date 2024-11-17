#include <iostream>

#include "window.hpp"
#include "display_image.hpp"

#include <vector>

static std::vector<display_image> display_images;

inline void trigger_image_load_dialogue() {
	image image{};
	if (load_image_from_dialogue(&image)) {
		display_image& display_image = display_images.emplace_back(image.path);
		create_display_image(&display_image, image);
	} else {
		std::cerr << "Failed to load image" << std::endl;
	}
}

inline void glfw_key_callback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
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

int main() {
	Window window{1000, 800, {0.45f, 0.55f, 0.60f, 1.0f}};

	//Initialization
	// stbi_set_flip_vertically_on_load(true);

	if (!window.setup(glfw_key_callback)) {
		std::cerr << "Failed to setup window" << std::endl;
		return -1;
	}

	NFD_Init();

	window.start_loop(
		[&]() {
			// Rendering
			for (const display_image& display_image : display_images) {
				draw_display_image(display_image, window);
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

	// Cleanup
	NFD_Quit();
	window.clean();
	glfwTerminate();

	return 0;
}
