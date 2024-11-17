#pragma once

#include <string>

#include "shaders/shader.hpp"

class ShaderProgram {
	public:

	private:
		uint32_t id;
		std::string name;

	public:
		explicit ShaderProgram(const std::string& name);
		~ShaderProgram();

		void attach_shader(const Shader& shader) const;
		bool link() const;
		bool check_compilation_status() const;

		void use() const;

		[[nodiscard]] uint32_t get_id() const;

	private:
};
