#pragma once

#include <string>
#include <unordered_map>

#include "glm/fwd.hpp"
#include "shaders/shader.hpp"

class ShaderProgram {
	public:

	private:
		uint32_t id;
		std::string name;
		std::unordered_map<std::string, int> uniform_locations;
		bool moved = false;

	public:
		explicit ShaderProgram(const std::string& name);
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;
		~ShaderProgram();

		void create();
		void attach_shader(const Shader& shader) const;
		bool link() const;
		bool check_compilation_status() const;

		void use() const;

		[[nodiscard]] uint32_t get_id() const;
		[[nodiscard]] int get_uniform_location(const std::string& name);

		void set_matrix4x4(const std::string& name, const glm::mat4& matrix);
		void set_float(const std::string& name, const float value);
		void set_int(const std::string& name, const int value);
		void set_vec4(const std::string& name, const glm::vec4& vec);

	private:
};
