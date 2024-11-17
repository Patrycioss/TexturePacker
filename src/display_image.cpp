#include "display_image.hpp"

#include <iostream>
#include <window.hpp>

DisplayImage::DisplayImage(const Image& image)
	: shader_program(image.path), texture(image), width(image.width), height(image.height) {
	if (image.width > 400) {
		const float factor = (float)image.width / 400;

		this->width = 400;
		this->height = std::floor((float)image.height / factor);
	} else if (image.height > 400) {
		const float factor = (float)image.height / 400;

		this->width = std::floor((float)image.width / factor);
		this->height = 400;
	} else {
		this->width = image.width;
		this->height = image.height;
	}
}

DisplayImage::DisplayImage(DisplayImage&& other) noexcept
	: shader_program(std::move(other.shader_program)), texture(std::move(other.texture)), width(other.width), height(other.height) {
}

DisplayImage::~DisplayImage() {
}

bool DisplayImage::create() {
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

	this->shader_program.attach_shader(vertexShader);
	this->shader_program.attach_shader(fragmentShader);

	if (!this->shader_program.link()) {
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

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	

	this->model_location = glGetUniformLocation(this->shader_program.get_id(), "model");
	this->projection_location = glGetUniformLocation(this->shader_program.get_id(), "projection");
	this->model = glm::mat4(1.0f);
	// display_image->model = glm::translate(display_image->model, glm::vec3(500,400, 0.0f));
	this->model = glm::scale(this->model, glm::vec3(width, height, 1.0f));

	return true;
}

void DisplayImage::draw(const Window& window) const {
	this->shader_program.use();

	glActiveTexture(GL_TEXTURE0);
	this->texture.bind();

	glUniformMatrix4fv(this->model_location, 1, false, &this->model[0][0]);
	glUniformMatrix4fv(this->projection_location, 1, false, &window.projection[0][0]);

	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
