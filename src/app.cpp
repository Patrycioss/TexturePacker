#include "app.hpp"

#include "input.hpp"
#include "collision_utils.hpp"

App* App::instance;

static void glfw_key_callback_invoker(GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
	App::get_instance()->glfw_key_callback(window, key, scancode, action, mods);
}

App::App(const int width, const int height, const ImVec4& background_color)
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
	if (!this->window.setup(glfw_key_callback_invoker)) {
		std::cerr << "Failed to setup window" << std::endl;
		throw std::runtime_error("Failed to setup window!");
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Input::initialize(&window);

	if (!this->renderer.initialize()) {
		throw std::runtime_error("Failed to initialize Renderer!");
	}

	renderer.enable_debug(true);

	NFD_Init();

	bool holdingLMB = false;

	this->window.start_loop(
		[&]() {
			// Input
			if (Input::get_key_pressed(GLFW_KEY_RIGHT)) {
				this->display_textures[0].set_position(this->display_textures[0].get_position() + glm::vec2{1.0f, 0.0f});
			}

			if (Input::get_mouse_button_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
				if (!holdingLMB) {
					holdingLMB = true;
					get_image_hovering_over();
				}

				if (this->has_selected_display_texture){
					this->selected_display_texture->set_position(Input::get_mouse_position() - this->selected_display_texture_offset);
				}
				
			} else {
				if (holdingLMB) {
					this->has_selected_display_texture = false;
					this->selected_display_texture = nullptr;
					holdingLMB = false;
				}
			}

			// Rendering
			int i = 0;
			for (DisplayTexture& display_image : this->display_textures) {
				i++;
				if (display_image.is_marked_dirty()) {
					display_image.recalculate_model();
				}
				this->renderer.draw_display_texture(display_image);
			}

			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Open", "Ctrl+O")) {
						this->trigger_image_load_dialogue();
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
		this->trigger_image_load_dialogue();
		return;
	}

	if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
		if (!this->display_textures.empty()) {
			this->display_textures.pop_back();
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
		this->display_textures.emplace_back(image);
	};
}

void App::get_image_hovering_over() {
	
	if (this->display_textures.empty()) {
		return;
	}
	
	const glm::vec2 mouse_position = Input::get_mouse_position();

	for (DisplayTexture& display_texture : this->display_textures) {
		// For now first come first serve, TODO: Probably don't do this
		const glm::vec2& position = display_texture.get_position();
		const glm::vec2& size = display_texture.get_size();
		if (collides_point_rectangle(mouse_position, position, size)) {
			this->selected_display_texture = &display_texture;
			this->has_selected_display_texture = true;
			this->selected_display_texture_offset = mouse_position - position;
			return;
		}
	}
}
