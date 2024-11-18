#include "display_image.hpp"

#include <iostream>
#include <window.hpp>

DisplayImage::DisplayImage(const Image& image)
	: texture(image), width(image.width), height(image.height) {
	if (image.width > 400) {
		const float factor = (float)image.width / 400;

		this->width = 400;
		this->height = std::floor((float)image.height / factor);
	} else if (image.height > 400) {
		const float factor = (float)image.height / 400;

		this->width = std::floor((float)image.width / factor);
		this->height = 400;
	} else {
		this->width = image.width;
		this->height = image.height;
	}

	this->model = glm::mat4x4(1.0f);
	this->model = glm::scale(this->model, glm::vec3(width, height, 1.0f));
}

DisplayImage::DisplayImage(DisplayImage&& other) noexcept
	: texture(std::move(other.texture)), width(other.width), height(other.height) {
}

DisplayImage::~DisplayImage() {
}

const glm::mat4x4& DisplayImage::get_model() const {
	return model;
}

const Texture& DisplayImage::get_texture() const {
	return texture;
}
