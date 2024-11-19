#include <window.hpp>

#include "renderer.hpp"
#include "matrix_debug.hpp"
#include "glm/gtc/type_ptr.inl"

Renderer::Renderer()
	: default_shader("default_renderer_shader_program"), debug_shader("debug_renderer_shader_program") {
}

Renderer::~Renderer() {
	glDeleteVertexArrays(1, &this->default_VAO);
	glDeleteVertexArrays(1, &this->debug_VAO);
}

bool Renderer::initialize() {
	if (!init_default_shader()) {
		std::cerr << "Failed to initialize default shader!" << std::endl;
		return false;
	}

	if (!init_debug_shader()) {
		std::cerr << "Failed to initialize debug shader!" << std::endl;
		return false;
	}

	return true;
}

void Renderer::enable_debug(const bool enable) {
	this->is_debug_enabled = enable;
}

void Renderer::draw_display_texture(const DisplayTexture& display_image) {
	this->default_shader.use();

	this->default_shader.set_matrix4x4("model", display_image.get_model());
	this->default_shader.set_matrix4x4("projection", Window::get_instance().projection);

	glActiveTexture(GL_TEXTURE0);
	display_image.get_texture().bind();

	glBindVertexArray(this->default_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	if (this->is_debug_enabled) {
		this->debug_shader.use();

		this->debug_shader.set_matrix4x4("model", display_image.get_model());
		this->debug_shader.set_matrix4x4("projection", Window::get_instance().projection);
		this->debug_shader.set_vec4("debug_color", display_image.get_debug_color());

		glBindVertexArray(this->debug_VAO);
		// Draw 8 for the 8 points to draw the lines between :)
		glDrawArrays(GL_LINES, 0, 8);
		glBindVertexArray(0);
	}
}

bool Renderer::init_default_shader() {
	this->default_shader.use();
	Shader vertexShader{Shader::Type::VERTEX_SHADER};
	if (!vertexShader.load(RES + "/shaders/default_vertex.glsl")) {
		std::cerr << "Failed to load default vertex shader so aborting draw_setup!" << std::endl;
		return false;
	};

	Shader fragmentShader{Shader::Type::FRAGMENT_SHADER};
	if (!fragmentShader.load(RES + "/shaders/default_fragment.glsl")) {
		std::cerr << "Failed to load default fragment shader so aborting draw_setup!" << std::endl;
		return false;
	}

	this->default_shader.create();
	this->default_shader.attach_shader(vertexShader);
	this->default_shader.attach_shader(fragmentShader);

	if (!this->default_shader.link()) {
		std::cerr << "Failed to link default program so aborting draw_setup!" << std::endl;
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

	glGenVertexArrays(1, &this->default_VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(this->default_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return true;
}

bool Renderer::init_debug_shader() {
	this->debug_shader.use();
	glLineWidth(4);

	Shader vertexShader{Shader::Type::VERTEX_SHADER};
	if (!vertexShader.load(RES + "/shaders/debug_vertex.glsl")) {
		std::cerr << "Failed to load debug vertex shader so aborting draw_setup!" << std::endl;
		return false;
	};

	Shader fragmentShader{Shader::Type::FRAGMENT_SHADER};
	if (!fragmentShader.load(RES + "/shaders/debug_fragment.glsl")) {
		std::cerr << "Failed to load debug fragment shader so aborting draw_setup!" << std::endl;
		return false;
	}

	this->debug_shader.create();
	this->debug_shader.attach_shader(vertexShader);
	this->debug_shader.attach_shader(fragmentShader);

	if (!this->debug_shader.link()) {
		std::cerr << "Failed to link debug program so aborting draw_setup!" << std::endl;
		return false;
	}

	constexpr float vertices[] = {
		1.0f, 0.0f, // top right
		1.0f, 1.0f, // bottom right

		0.0f, 1.0f, // bottom left  
		1.0f, 1.0f, // bottom right

		0.0f, 0.0f, // top left
		0.0f, 1.0f, // bottom left

		0.0f, 0.0f, // top left
		1.0f, 0.0f, // top right
	};

	uint32_t VBO, EBO;

	glGenVertexArrays(1, &debug_VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(debug_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	return true;
}
