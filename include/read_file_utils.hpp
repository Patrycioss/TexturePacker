#pragma once

#include <fstream>
#include <sstream>
#include <iostream>

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