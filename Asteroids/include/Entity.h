#define STRICT

#ifndef _ENTITY_H
#define _ENTITY_H

#pragma once

#include "SFML/Graphics.hpp"

#include "Object.h"

class Entity : public Object
{
public:
	enum class Type : int
	{
		Player,
		Projectile,
		Asteroid,
		Enemy_Spaceship,
		Unknown
	};

	enum class Owner : int
	{
		Engine,
		Player,
		Asteroid,
		Enemy_Spaceship,
		Unknown
	};

	explicit Entity(const int _id, const int id, Entity::Type type = Type::Unknown, Entity::Owner owner = Owner::Unknown)
	{
		this->_id = _id;
		this->_type = Object::Type::Entity;

		this->id = id;
		this->type = type;
		this->owner = owner;
	};
	~Entity() {};

	int id = 0;
	int spawn_cooldown = 0;

	Type spawn = Type::Unknown;

	bool spawned_inheritance = false;

	Type type = Type::Unknown;
	Owner owner = Owner::Unknown;

	Entity::Owner generate_ownership();

	virtual void calc_move(float elapsed_time);
};

#endif // !_ENTITY_H
