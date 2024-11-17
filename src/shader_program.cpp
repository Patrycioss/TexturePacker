#include <iostream>

#include "shader_program.hpp"

ShaderProgram::ShaderProgram(const std::string& name)
	: id(glCreateProgram()), name(name) {
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(this->id);
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
		char infoLog[512];
		glGetProgramInfoLog(this->id, 512, nullptr, infoLog);
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