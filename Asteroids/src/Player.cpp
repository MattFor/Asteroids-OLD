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
		this->spawn_cooldown = 30;
		this->spawn = Spawnable::Projectile;
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
		this->rotation += -_rotation;
	}
	else if (input(RIGHT))
	{
		const float _rotation = (SLIDE_ROTATION ? 175.0f * elapsed_time * elapsed_time : 2.0f);
		this->rotation += _rotation;
	}

	// Clamping speed and applying decay
	this->dx = std::clamp(this->dx * DECAY, -MAX_SPEED, MAX_SPEED);
	this->dy = std::clamp(this->dy * DECAY, -MAX_SPEED, MAX_SPEED);

	// Adjusting rotation
	this->angle += this->rotation;
	this->rotation *= (SLIDE_ROTATION ? DECAY : 0.0f);

	// Normalizing angle to [0, 360)
	this->angle = std::fmod(this->angle, 360.0f);
	if (this->angle < 0.0f) this->angle += 360.0f;

	this->x += this->dx * elapsed_time;
	this->y += this->dy * elapsed_time;

	if (DEBUG_PLAYER_INFO)
	{
		printf("X[%.3f] Y[%.3f] A[%.3f] DX[%.3f] DY[%.3f] R[%.3f] SC[%d] SP[%d]\n", this->x, this->y, this->angle, this->dx, this->dy, this->rotation, this->spawn_cooldown, (int)this->spawn);
	}
};
