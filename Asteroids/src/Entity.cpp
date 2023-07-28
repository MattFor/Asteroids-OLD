#define STRICT

#include "Entity.h"

void Entity::calc_move(float elapsed_time)
{
	const float radians_per_degree = 3.14159f / 180.0f;
	float angle_radians = this->angle * radians_per_degree;

	this->x += this->dx * elapsed_time;
	this->y += this->dy * elapsed_time;

	float dx_temp = std::sin(angle_radians) * 20.0f * elapsed_time;
	float dy_temp = -std::cos(angle_radians) * 20.0f * elapsed_time;

	const float adjustment_factor = 0.1f;
	if (std::abs(this->dx - dx_temp) > 0.01f || std::abs(this->dy - dy_temp) > 0.01f) {
		this->dx += (dx_temp - this->dx) * adjustment_factor;
		this->dy += (dy_temp - this->dy) * adjustment_factor;
	}
};
