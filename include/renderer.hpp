#pragma once

#include <display_texture.hpp>
#include <shaders/shader_program.hpp>

class Renderer {
	private:
		uint32_t VAO{};
		ShaderProgram default_shader;
		int model_location{};
		int projection_location{};

	public:
		Renderer();
		~Renderer();

		bool initialize();
		void draw_display_texture(const DisplayTexture& display_image) const;
};
