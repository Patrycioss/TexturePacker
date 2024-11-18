#pragma once
#include "image.hpp"

class Texture {
	public:

	private:
		unsigned int id{};
		bool moved = false;

	public:
		explicit Texture(const Image& image);
		Texture(Texture&& texture) noexcept;
		~Texture();

		void bind() const;

		[[nodiscard]] unsigned int get_id() const;

	private:
};
