#pragma once

enum shader_type { VERTEX_SHADER = GL_VERTEX_SHADER, FRAGMENT_SHADER = GL_FRAGMENT_SHADER };

inline bool check_shader_compilation(const uint32_t shader, const std::string& name) {
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "Shader with name: '" << name << "' failed! Info: '\n" << infoLog << "'" << std::endl;
		return false;
	}

	return true;
}

inline bool check_program_compilation(const uint32_t program, const std::string& name) {
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cerr << "ShaderProgram with name: '" << name << "' failed! Info: '\n" << infoLog << "'" << std::endl;
		return false;
	}

	return true;
}

inline bool read_file_to_string(const std::string& path, std::string* output) {
	std::ifstream t(path);

	if (!t.good()) {
		std::cerr << "Could not open file " << path << std::endl;
		return false;
	}

	std::stringstream buffer;
	buffer << t.rdbuf();
	*output = buffer.str();
	return true;
}

inline bool load_shader(int* shader, const shader_type type, const std::string& path, const std::string& name) {
	std::string source;

	if (read_file_to_string(path, &source)) {
		*shader = glCreateShader(type);
		const char* src_str = source.c_str();

		glShaderSource(*shader, 1, &src_str, nullptr);
		glCompileShader(*shader);

		if (!check_shader_compilation(*shader, name)) {
			glDeleteShader(*shader);
			return false;
		}

		std::cout << "Vertex shader compiled successfully! From source: \n" << source << "\n" << std::endl;
		return true;
	}
	return false;
}

inline bool create_shader_program(uint32_t* program, int shaders[], const int shader_count, const std::string& name) {
	*program = glCreateProgram();

	for (int i = 0; i < shader_count; i++) {
		glAttachShader(*program, shaders[i]);
	}

	glLinkProgram(*program);

	if (!check_program_compilation(*program, name)) {
		glDeleteProgram(*program);
		return false;
	}

	return true;
}
