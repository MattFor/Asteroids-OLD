#define STRICT

#include "Entity.h"

void Entity::calc_move(float elapsed_time)
{
	const float radians_per_degree = 3.14159f / 180.0f;
	float angle_radians = this->get_angle() * radians_per_degree;

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

Entity::Owner Entity::generate_ownership()
{
	switch (this->type)
	{
	case Type::Player:
		return Owner::Player;

	case Type::Enemy_Spaceship:
		return Owner::Enemy_Spaceship;

	case Type::Projectile:
		// Projectiles should not be able to spawn projectiles.
		return Owner::Unknown;

	default:
		return Owner::Unknown;
	}
}