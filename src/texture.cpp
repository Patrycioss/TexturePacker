#include "texture.hpp"

#include <glad/glad.h>

Texture::Texture(const Image& image) {
	glGenTextures(1, &this->id);
	bind();

	// Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// MipMaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (image.image_type == Image::rgb) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
	} else if (image.image_type == Image::rgba) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
	}
	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(Texture&& texture) noexcept
	: id(texture.id) {
	texture.moved = true;
}

Texture::~Texture() {
	if (!moved) {
		std::cout << "Deleting texture with id: " << id << std::endl;
		glDeleteTextures(1, &this->id);
	}
}

void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, this->id);
}

unsigned int Texture::get_id() const {
	return this->id;
}
