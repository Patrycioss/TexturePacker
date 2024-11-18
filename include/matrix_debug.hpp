#pragma once

#include <string>

inline std::string matrix_to_string(const glm::mat4x4& matrix) {
	std::stringstream ss;

	ss << "\n";
	ss << matrix[0][0] << " " << matrix[0][1] << " " << matrix[0][2] << " " << matrix[0][3] << "\n";
	ss << matrix[1][0] << " " << matrix[1][1] << " " << matrix[1][2] << " " << matrix[1][3] << "\n";
	ss << matrix[2][0] << " " << matrix[2][1] << " " << matrix[2][2] << " " << matrix[2][3] << "\n";
	ss << matrix[3][0] << " " << matrix[3][1] << " " << matrix[3][2] << " " << matrix[3][3] << "\n";
	ss << "\n";

	return ss.str();
}
