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
		this->spawn = Spawnable::Projectile;
		this->spawn_cooldown = 30;
	}

	if (input(UP))
	{
		const float angle_radians = this->angle * RADIANS;
		const float speed = MOVEMENT_CONSTANT * elapsed_time;

		this->dx += std::sin(angle_radians) * speed;
		this->dy -= std::cos(angle_radians) * speed;
	}

	if (input(LEFT))
	{
		const float _rotation = (SLIDE_ROTATION ? 175.0f * elapsed_time * elapsed_time : 2.0f);
		this->rotation += (OLD_SCHOOL ? _rotation : -_rotation);
	}
	else if (input(RIGHT))
	{
		const float _rotation = (SLIDE_ROTATION ? 175.0f * elapsed_time * elapsed_time : 2.0f);
		this->rotation += (OLD_SCHOOL ? -_rotation : _rotation);
	}

	// Max speed adjustments
	const float _MAX_SPEED = (OLD_SCHOOL ? MAX_SPEED * 0.2f : MAX_SPEED);

	// Clamping speed and applying decay
	this->dx = std::clamp(this->dx * DECAY, -_MAX_SPEED, _MAX_SPEED);
	this->dy = std::clamp(this->dy * DECAY, -_MAX_SPEED, _MAX_SPEED);

	// Adjusting rotation
	this->angle += (OLD_SCHOOL ? this->rotation * 0.5f : this->rotation);
	this->rotation *= (SLIDE_ROTATION ? DECAY : 0.0f);

	// Normalizing angle to [0, 360)
	this->angle = std::fmod(this->angle, 360.0f);
	if (this->angle < 0.0f) this->angle += 360.0f;

	this->x += this->dx * elapsed_time;
	this->y += this->dy * elapsed_time;
};
