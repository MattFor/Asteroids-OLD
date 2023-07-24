#define STRICT

#ifndef _PLAYER_H
#define _PLAYER_H

#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	using Entity::Entity;

	int lives = 3;

	float MAX_VELOCITY = 0;
	float MAX_ROTATION = 0;

	int shoot_cooldown = 0;
};

#endif // !_PLAYER_H
