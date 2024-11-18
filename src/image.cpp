#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image::Image(): image_type(), width(0), height(0), channels(0), data(nullptr) {
}

Image::~Image() {
	stbi_image_free(this->data);
}

bool Image::load(const std::string& path) {
	const std::filesystem::path extension = std::filesystem::path(path).extension();

	this->path = path;

	if (extension == ".jpg" || extension == ".jpeg") {
		this->data = stbi_load(path.c_str(), &this->width, &this->height, &this->channels, 3);
		this->image_type = Image::rgb;
	} else if (extension == ".png") {
		this->data = stbi_load(path.c_str(), &this->width, &this->height, &this->channels, 4);
		this->image_type = Image::rgba;
	} else {
		std::cerr << "Unsupported image type with extension: " << extension << std::endl;
		return false;
	}

	if (!this->data) {
		std::cerr << "Failed to load image with path: " << path << std::endl;
		return false;
	}
	return true;
}
