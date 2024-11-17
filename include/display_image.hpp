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
		uint32_t VAO{};
		ShaderProgram shader_program;
		Texture texture;
		int model_location{}, projection_location{};
		glm::mat4x4 model{1.0f};

		int width, height;

	public:
		explicit DisplayImage(const Image& image);
		DisplayImage(DisplayImage&& other) noexcept;
		DisplayImage(DisplayImage& other)= delete;
		~DisplayImage();

		bool create();
		void draw(const Window& window) const;

	private:
};
