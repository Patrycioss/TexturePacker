#pragma once

#include <display_texture.hpp>
#include <shaders/shader_program.hpp>

class Renderer {
	private:
		uint32_t default_VAO{}, debug_VAO{};
		ShaderProgram default_shader;
		ShaderProgram debug_shader;

		bool is_debug_enabled = false;

	public:
		Renderer();
		~Renderer();

		bool initialize();
		void enable_debug(bool enable);
		void draw_display_texture(const DisplayTexture& display_image);

	private:
		bool init_default_shader();
		bool init_debug_shader();
};