#pragma once

#define RES std::string(RESOURCES)

#include <fstream>
#include <glad/glad.h>

#include "image.hpp"
#include "display_image.hpp"
#include "shader_program.hpp"
#include "texture_creation.hpp"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"

struct display_image {
	uint32_t VAO;
	ShaderProgram shader_program;
	uint32_t texture;
	int model_location, projection_location;
	glm::mat4x4 model;

	explicit display_image(const std::string& name)
		: shader_program(name) {
	}
};

inline bool create_display_image(display_image* display_image, const image& image) {
	const Shader vertexShader{Shader::Type::VERTEX_SHADER};
	if (!vertexShader.load(RES + "/shaders/vertex.glsl")) {
		std::cerr << "Failed to load vertex shader so aborting draw_setup!" << std::endl;
		return false;
	};

	const Shader fragmentShader{Shader::Type::FRAGMENT_SHADER};
	if (!fragmentShader.load(RES + "/shaders/fragment.glsl")) {
		std::cerr << "Failed to load fragment shader so aborting draw_setup!" << std::endl;
		return false;
	}

	display_image->shader_program.attach_shader(vertexShader);
	display_image->shader_program.attach_shader(fragmentShader);

	if (!display_image->shader_program.link()) {
		std::cerr << "Failed to link program so aborting draw_setup!" << std::endl;
		return false;
	}

	constexpr float vertices[] = {
		// positions            // texture coordinates
		1.0f, 0.0f, 1.0f, 0.0f, // top right
		1.0f, 1.0f, 1.0f, 1.0f, // bottom right
		0.0f, 1.0f, 0.0f, 1.0f, // bottom left
		0.0f, 0.0f, 0.0f, 0.0f // top left 
	};

	constexpr unsigned int indices[] = {
		0, 1, 3, // first Triangle
		1, 2, 3 // second Triangle
	};

	uint32_t VBO, EBO;

	glGenVertexArrays(1, &display_image->VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(display_image->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	int width, height;
	if (image.width > 400) {
		float factor = (float)image.width / 400;

		width = 400;
		height = std::floor((float)image.height / factor);
	} else if (image.height > 400) {
		float factor = (float)image.height / 400;

		width = std::floor((float)image.width / factor);
		height = 400;
	} else {
		width = image.width;
		height = image.height;
	}

	display_image->model_location = glGetUniformLocation(display_image->shader_program.get_id(), "model");
	display_image->projection_location = glGetUniformLocation(display_image->shader_program.get_id(), "projection");
	display_image->model = glm::mat4(1.0f);
	// display_image->model = glm::translate(display_image->model, glm::vec3(500,400, 0.0f));
	display_image->model = glm::scale(display_image->model, glm::vec3(width, height, 1.0f));

	create_texture(&display_image->texture, image);
	return true;
}

inline void draw_display_image(const display_image& display_image, const Window& window) {
	display_image.shader_program.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, display_image.texture);

	glUniformMatrix4fv(display_image.model_location, 1, false, &display_image.model[0][0]);
	glUniformMatrix4fv(display_image.projection_location, 1, false, &window.projection[0][0]);

	glBindVertexArray(display_image.VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
