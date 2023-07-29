#define STRICT

#include "Entity.h"

void Entity::move()
{
	if (this->shape != nullptr)
	{
		this->shape->setPosition(this->x, this->y);
		return;
	}

	this->sprite->setPosition(this->x, this->y);
};

void Entity::rotate()
{
	if (this->shape != nullptr)
	{
		this->shape->setRotation(this->angle);
		return;
	}

	this->sprite->setRotation(this->angle);
};

void Entity::calc_move(float elapsed_time)
{
	const float angle_radians = this->angle * RADIANS;

	// Calculate the directional velocities.
	const float carried_dx = this->dx + std::sin(angle_radians) * 1000.0f;
	const float carried_dy = this->dy + -std::cos(angle_radians) * 1000.0f;

	// Update position based on directional velocities and elapsed time.
	this->x += carried_dx * elapsed_time;
	this->y += carried_dy * elapsed_time;
};
