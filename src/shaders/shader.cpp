#pragma once

#include "shaders/shader.hpp"
#include "read_file_utils.hpp"

Shader::Shader(const Type type)
	: id(glCreateShader((GLenum)type)), type(type) {
}

Shader::~Shader() {
	glDeleteShader(this->id);
}

bool Shader::load(const std::string& path) const {
	std::string source;

	if (read_file_to_string(path, &source)) {
		const char* src_str = source.c_str();

		glShaderSource(this->id, 1, &src_str, nullptr);
		glCompileShader(this->id);

		return check_shader_compilation();
	}
	return false;
}

bool Shader::check_shader_compilation() const {
	const char* name;
	switch (this->type) {
		case Type::VERTEX_SHADER:
			name = "Vertex shader";
			break;
		case Type::FRAGMENT_SHADER:
			name = "Fragment shader";
			break;
		default:
			name = "Shader Type without string case";
	}

	int success;
	glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);

	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(this->id, 512, nullptr, infoLog);
		std::cerr << "Shader with name: '" << name << "' failed to compile! Info: '\n" << infoLog << "'" << std::endl;
		return false;
	}

	return true;
}

const Shader::Type& Shader::get_type() const {
	return this->type;
}

uint32_t Shader::get_id() const {
	return this->id;
}
