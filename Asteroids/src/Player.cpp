#define STRICT

#include "Player.h"

bool Player::input(sf::Keyboard::Key key)
{
	return sf::Keyboard::isKeyPressed(key);
};

void Player::calc_move(float elapsed_time)
{
	if (input(SHOOT) && this->spawn_cooldown <= 0)
	{
		this->spawn = SpawnableType::Projectile;
		this->spawn_cooldown = 35;
	}

	const float angle_radians = this->angle * RADIANS;

	if (input(UP))
	{
		// We accumulate dx and dy over time for gradual movement
		this->dx += std::sin(angle_radians) * 1000.0f * elapsed_time;
		this->dy -= std::cos(angle_radians) * 1000.0f * elapsed_time;
	}

	if (input(LEFT))
	{
		const float _rotation = (SLIDE_ROTATION ? 175.0f * elapsed_time * elapsed_time : 4.0f);

		if (OLD_SCHOOL)
		{
			this->rotation += _rotation;
		}
		else
		{
			this->rotation -= _rotation;
		}
	}

	if (input(RIGHT))
	{
		const float _rotation = (SLIDE_ROTATION ? 175.0f * elapsed_time * elapsed_time : 4.0f);

		if (OLD_SCHOOL)
		{
			this->rotation -= _rotation;
		}
		else
		{
			this->rotation += _rotation;
		}
	}

	float _MAX_SPEED = MAX_SPEED;

	if (OLD_SCHOOL)
	{
		_MAX_SPEED *= 0.3f;
	}

	this->dx = std::clamp(this->dx, -_MAX_SPEED, _MAX_SPEED);
	this->dy = std::clamp(this->dy, -_MAX_SPEED, _MAX_SPEED);

	// apply a friction-like effect to gradually reduce velocity and rotation when no input
	this->dx *= DECAY;
	this->dy *= DECAY;

	this->angle += this->rotation;

	if (SLIDE_ROTATION)
	{
		this->rotation *= DECAY;
	}
	else
	{
		this->rotation = 0.0f;
	}

	// normalize the angle to [0, 360)
	while (this->angle >= 360.0f) this->angle -= 360.0f;
	while (this->angle < 0.0f) this->angle += 360.0f;

	this->x += this->dx * elapsed_time;
	this->y += this->dy * elapsed_time;
};
