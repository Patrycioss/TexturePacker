#include <window.hpp>

#include "renderer.hpp"
#include "matrix_debug.hpp"

Renderer::Renderer()
	: default_shader("default_renderer_shader_program") {
}

Renderer::~Renderer() {
	glDeleteVertexArrays(1, &this->VAO);
}

bool Renderer::initialize() {
	Shader vertexShader{Shader::Type::VERTEX_SHADER};
	if (!vertexShader.load(RES + "/shaders/vertex.glsl")) {
		std::cerr << "Failed to load vertex shader so aborting draw_setup!" << std::endl;
		return false;
	};

	Shader fragmentShader{Shader::Type::FRAGMENT_SHADER};
	if (!fragmentShader.load(RES + "/shaders/fragment.glsl")) {
		std::cerr << "Failed to load fragment shader so aborting draw_setup!" << std::endl;
		return false;
	}

	this->default_shader.create();
	this->default_shader.attach_shader(vertexShader);
	this->default_shader.attach_shader(fragmentShader);

	if (!this->default_shader.link()) {
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

	this->model_location = glGetUniformLocation(this->default_shader.get_id(), "model");
	this->projection_location = glGetUniformLocation(this->default_shader.get_id(), "projection");
	return true;
}

void Renderer::draw_display_texture(const DisplayTexture& display_image) const {
	this->default_shader.use();

	glActiveTexture(GL_TEXTURE0);
	display_image.get_texture().bind();

	// std::cout << matrix_to_string(display_image.get_model()) << std::endl;

	glUniformMatrix4fv(this->model_location, 1, false, &display_image.get_model()[0][0]);
	glUniformMatrix4fv(this->projection_location, 1, false, &Window::get_instance().projection[0][0]);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
