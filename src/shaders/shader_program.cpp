﻿#include <iostream>

#include "shaders/shader_program.hpp"

#include <Input.hpp>

#include "glm/gtc/type_ptr.hpp"

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

int ShaderProgram::get_uniform_location(const std::string& name) {
	if (uniform_locations.contains(name)) {
		return uniform_locations.at(name);
	}

	const int location = glGetUniformLocation(id, name.c_str());
	if (location == -1) {
		std::cerr << "Failed to get uniform location with name: " << name << std::endl;
		return location;
	}

	uniform_locations.emplace(name, location);
	return location;
}

void ShaderProgram::set_matrix4x4(const std::string& name, const glm::mat4x4& matrix) {
	glUniformMatrix4fv(this->get_uniform_location(name), 1, GL_FALSE, &matrix[0][0]);
}

void ShaderProgram::set_float(const std::string& name, const float value) {
	glUniform1f(this->get_uniform_location(name), value);
}

void ShaderProgram::set_int(const std::string& name, const int value) {
	glUniform1i(this->get_uniform_location(name), value);
}

void ShaderProgram::set_vec4(const std::string& name, const glm::vec4& vec) {
	glUniform4fv(this->get_uniform_location(name), 1, glm::value_ptr(vec));
}