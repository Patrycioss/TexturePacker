#include <iostream>

#include "shaders/shader_program.hpp"

ShaderProgram::ShaderProgram(const std::string& name)
	: id(), name(name) {
}

ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
	: id(shaderProgram.id), name(std::move(shaderProgram.name)) {
	shaderProgram.moved = true;
}

ShaderProgram::~ShaderProgram() {
	if (moved != true) {
		std::cout << "Deleting shader program: " << name << std::endl;
		glDeleteProgram(this->id);
	}
}

void ShaderProgram::create() {
	this->id = glCreateProgram();
}

void ShaderProgram::attach_shader(const Shader& shader) const {
	glAttachShader(this->id, shader.get_id());
}

bool ShaderProgram::link() const {
	glLinkProgram(this->id);

	if (!check_compilation_status()) {
		glDeleteProgram(this->id);
		return false;
	}

	return true;
}

bool ShaderProgram::check_compilation_status() const {
	int success;
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);

	if (!success) {
		char infoLog[1024];
		glGetProgramInfoLog(this->id, 1024, nullptr, infoLog);
		std::cerr << "ShaderProgram with name: '" << this->name << "' failed! Info: '\n" << infoLog << "'" << std::endl;
		return false;
	}

	return true;
}

void ShaderProgram::use() const {
	glUseProgram(this->id);
}

uint32_t ShaderProgram::get_id() const {
	return this->id;
}
