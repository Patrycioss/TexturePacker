#pragma once

#include <renderer.hpp>
#include <vector>

#include "window.hpp"
#include "display_texture.hpp"

class App {
	public:

	private:
		static App* instance;
		std::vector<DisplayTexture> display_textures{};
		Window window;
		Renderer renderer;

		bool has_selected_display_texture;
		DisplayTexture* selected_display_texture = nullptr;
		glm::vec2 selected_display_texture_offset;

		const nfdu8filteritem_t image_filters[1]{"Images code", "png,jpg,jpeg"};
		const nfdopendialogu8args_t image_open_args = {image_filters, 1};

	public:
		App(int width, int height, const ImVec4& background_color);
		App(const App&) = delete;
		App(const App&&) = delete;
		~App();

		void start();
		void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		[[nodiscard]] static App* get_instance();

	private:
		void trigger_image_load_dialogue();
		void get_image_hovering_over();
};