#include "display_texture.hpp"

#include <iostream>
#include <window.hpp>

DisplayTexture::DisplayTexture(const Image& image)
	: original_size(image.width, image.height), texture(image), size(this->original_size) {
	if (image.width > 400) {
		const float factor = (float)this->original_size.x / 400;

		this->size.x = 400;
		this->size.y = std::floor((float)this->original_size.y / factor);
	} else if (this->original_size.y > 400) {
		const float factor = (float)this->original_size.y / 400;

		this->size.x = std::floor((float)this->original_size.x / factor);
		this->size.y = 400;
	}

	recalculate_model();


}

DisplayTexture::DisplayTexture(DisplayTexture&& other) noexcept
	: original_size(other.original_size), model(std::move(other.model)), texture(std::move(other.texture)), position(other.position),
	  size(other.size),
	  rotation(other.rotation) {
}

DisplayTexture::~DisplayTexture() {
}

void DisplayTexture::set_position(const glm::vec2& position) {
	this->position = position;
	dirty = true;
}

void DisplayTexture::set_size(const glm::ivec2& size) {
	this->size = size;
	dirty = true;
}

void DisplayTexture::set_rotation(float rotation) {
	this->rotation = rotation;
	dirty = true;
}

void DisplayTexture::recalculate_model() {
	this->model = glm::mat4x4(1.0f);
	this->model = glm::translate(this->model, glm::vec3(this->position.x, this->position.y, 0));
	this->model = glm::scale(this->model, glm::vec3(this->size.x, this->size.y, 1.0f));
	this->model = glm::rotate(this->model, this->rotation, glm::vec3(0, 0, 1));
	dirty = false;
}

const glm::mat4x4& DisplayTexture::get_model() const {
	return model;
}

const Texture& DisplayTexture::get_texture() const {
	return texture;
}

const bool& DisplayTexture::is_marked_dirty() const {
	return dirty;
}

const glm::vec2& DisplayTexture::get_position() const {
	return position;
}

const glm::ivec2& DisplayTexture::get_size() const {
	return size;
}

const float& DisplayTexture::get_rotation() const {
	return rotation;
}

const glm::ivec2& DisplayTexture::get_original_size() const {
	return original_size;
}
