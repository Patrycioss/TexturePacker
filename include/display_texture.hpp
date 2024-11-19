#pragma once

#define RES std::string(RESOURCES)

#include <imgui.h>

#include "texture.hpp"
#include "shaders/shader_program.hpp"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"

class Window;

class DisplayTexture {
	public:

	private:
		const glm::ivec2 original_size;
		glm::mat4x4 model{1.0f};
		Texture texture;
		bool dirty = false;

		glm::vec2 position{0,0};
		glm::ivec2 size;
		float rotation{0};

		glm::vec4 debug_color{};

	public:
		explicit DisplayTexture(const Image& image);
		DisplayTexture(DisplayTexture&& other) noexcept;
		DisplayTexture(DisplayTexture& other) = delete;
		~DisplayTexture();

		void set_position(const glm::vec2& position);
		void set_size(const glm::ivec2& size);
		void set_rotation(float rotation);

		void recalculate_model();

		const glm::mat4x4& get_model() const;
		const Texture& get_texture() const;
		const bool& is_marked_dirty() const;

		const glm::vec2& get_position() const;
		const glm::ivec2& get_size() const;
		const float& get_rotation() const;

		const glm::ivec2& get_original_size() const;
		const glm::vec4& get_debug_color() const;
	private:
};
