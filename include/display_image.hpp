#pragma once

#define RES std::string(RESOURCES)

#include <fstream>
#include <glad/glad.h>

#include "image.hpp"
#include "display_image.hpp"
#include "shader_loading.hpp"
#include "texture_creation.hpp"


struct display_image {
	uint32_t VAO;
	uint32_t shader_program;
	uint32_t texture;
};

inline bool create_display_image(display_image* display_image, const image& image) {
	int shaders[2]{};

	if (!load_shader(&shaders[0], VERTEX_SHADER, RES + "/shaders/vertex.glsl", "vertex.glsl")) {
		std::cerr << "Failed to load vertex shader so aborting draw_setup!" << std::endl;
		return false;
	}

	if (!load_shader(&shaders[1], FRAGMENT_SHADER, RES + "/shaders/fragment.glsl", "fragment.glsl")) {
		std::cerr << "Failed to load fragment shader so aborting draw_setup!" << std::endl;
		return false;
	}

	create_shader_program(&display_image->shader_program, shaders, 2, "Shader Program");
	std::cout << "Successfully created shader program!" << std::endl;

	constexpr float vertices[] = {
		// positions            // texture coordinates
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f // top left 
	};

	constexpr unsigned int indices[] = {
		// note that we start from 0!
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	create_texture(&display_image->texture, image);
	return true;
}

inline void draw_display_image(const display_image& display_image) {
	glUseProgram(display_image.shader_program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, display_image.texture);

	glBindVertexArray(display_image.VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
