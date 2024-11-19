#pragma once

#include <string>
#include <glad/glad.h>

class Shader {
	public:
		enum class Type {
			VERTEX_SHADER = GL_VERTEX_SHADER,
			FRAGMENT_SHADER = GL_FRAGMENT_SHADER
		};

	private:
		uint32_t id{};
		Type type;

	public:
		explicit Shader(Type type);
		~Shader();

		bool load(const std::string& path);
		bool check_shader_compilation() const;

		[[nodiscard]] const Type& get_type() const;
		[[nodiscard]] uint32_t get_id() const;

	private:
};
