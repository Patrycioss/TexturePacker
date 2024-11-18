#include "display_texture.hpp"

#include <iostream>
#include <window.hpp>

DisplayTexture::DisplayTexture(const Image& image)
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

DisplayTexture::DisplayTexture(DisplayTexture&& other) noexcept
	: texture(std::move(other.texture)), width(other.width), height(other.height), model(std::move(other.model)) {
}

DisplayTexture::~DisplayTexture() {
}

const glm::mat4x4& DisplayTexture::get_model() const {
	return model;
}

const Texture& DisplayTexture::get_texture() const {
	return texture;
}
