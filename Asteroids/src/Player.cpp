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
		this->spawn = Type::Projectile;
		this->spawn_cooldown = 35;
	}

	const float angle_radians = this->get_angle() * RADIANS;

	const float dx_temp = std::sin(angle_radians) * 10000.0f * elapsed_time;
	const float dy_temp = -std::cos(angle_radians) * 10000.0f * elapsed_time;

	if (input(UP))
	{
		this->dx += dx_temp - this->dx;
		this->dy += dy_temp - this->dy;
	}

	if (input(LEFT))
	{
		this->rotation += 150.0f * elapsed_time;
	}
	
	if (input(RIGHT))
	{
		this->rotation -= 150.0f * elapsed_time;
	}

	this->angle += this->rotation * elapsed_time;

	// normalize the angle to [0, 360)
	while (this->angle >= 360.0f) this->angle -= 360.0f;
	while (this->angle < 0.0f) this->angle += 360.0f;

	this->dx = std::clamp(this->dx, -MAX_SPEED, MAX_SPEED);
	this->dy = std::clamp(this->dy, -MAX_SPEED, MAX_SPEED);

	this->dx *= DECAY;
	this->dy *= DECAY;
	this->rotation *= DECAY;

	this->x += this->dx * elapsed_time;
	this->y += this->dy * elapsed_time;
};