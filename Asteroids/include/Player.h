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

	Type spawn = Type::Unknown;

	bool spawned_inheritance = true;

	void calc_move(float elapsed_time);

private:
	bool input(sf::Keyboard::Key);
};

#endif // !_PLAYER_H
