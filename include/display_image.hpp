#pragma once

#define RES std::string(RESOURCES)

#include "texture.hpp"
#include "shaders/shader_program.hpp"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"

class Window;

class DisplayImage {
	public:

	private:
		glm::mat4x4 model{1.0f};
		Texture texture;

		int width;
		int height;

		bool moved = false;

	public:
		explicit DisplayImage(const Image& image);
		DisplayImage(DisplayImage&& other) noexcept;
		DisplayImage(DisplayImage& other) = delete;
		~DisplayImage();

		const glm::mat4x4& get_model() const;
		const Texture& get_texture() const;

	private:
};
