#pragma once

#include <glm/vec2.hpp>

inline bool collides_point_rectangle(const glm::vec2& point, const glm::vec2& rect_position, const glm::vec2& rect_size) {
	return point.x >= rect_position.x
		&& point.x <= rect_position.x + rect_size.x
		&& point.y >= rect_position.y
		&& point.y <= rect_position.y + rect_size.y;
}