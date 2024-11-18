#pragma once

#define RES std::string(RESOURCES)

#include "texture.hpp"
#include "shaders/shader_program.hpp"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"

class Window;

class DisplayTexture {
	public:

	private:
		glm::mat4x4 model{1.0f};
		Texture texture;

		int width;
		int height;

		bool moved = false;

	public:
		explicit DisplayTexture(const Image& image);
		DisplayTexture(DisplayTexture&& other) noexcept;
		DisplayTexture(DisplayTexture& other) = delete;
		~DisplayTexture();

		const glm::mat4x4& get_model() const;
		const Texture& get_texture() const;

	private:
};
