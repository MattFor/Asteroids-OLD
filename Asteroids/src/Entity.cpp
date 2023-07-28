#define STRICT

#include "Entity.h"

void Entity::calc_move(float elapsed_time)
{
	const float radians_per_degree = 3.14159f / 180.0f;
	float angle_radians = this->angle * radians_per_degree;

	// Calculate the directional velocities.
	this->dx = std::sin(angle_radians) * 1000.0f;
	this->dy = -std::cos(angle_radians) * 1000.0f;

	// Update position based on directional velocities and elapsed time.
	this->x += this->dx * elapsed_time;
	this->y += this->dy * elapsed_time;
};
