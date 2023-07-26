#define STRICT

#ifndef _ENTITY_H
#define _ENTITY_H

#pragma once

#include "SFML/Graphics.hpp"

#include "Object.h"

class Entity : public Object
{
public:
	explicit Entity(
		const int _id,
		const int id,
		Spawnable type = Spawnable::Unknown,
		Spawnable owner = Spawnable::Unknown
	)
	{
		this->_id = _id;
		this->_type = Object::ElementType::Dynamic;

		this->id = id;
		this->type = type;
		this->owner = owner;
	};
	~Entity() {};

	int id = 0;

	// Gameplay
	int spawn_cooldown = 0;

	bool spawned_inheritance = false;

	Spawnable type = Spawnable::Unknown;
	Spawnable owner = Spawnable::Unknown;
	Spawnable spawn = Spawnable::Unknown;

	// Movement
	virtual void calc_move(float elapsed_time);
};

#endif // !_ENTITY_H
