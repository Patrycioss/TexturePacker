#pragma once

#include <filesystem>

#include "file_dialogue.hpp"

class DisplayImage;

struct Image {
	enum image_type {
		rgb,
		rgba,
	} image_type;

	int width, height, channels;
	std::string path;
	unsigned char* data;

	Image();
	~Image();

	bool load(const std::string& path);
};