#define STRICT

#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#pragma once

#include "Entity.h"

class Projectile : public Entity
{
public:
	using Entity::Entity;

	// Gameplay
	int lifetime = 0;

	// Graphics
	sf::CircleShape* shape = new sf::CircleShape();

	void set_shape(const float, sf::Color = sf::Color::Yellow);
};

#endif _PROJECTILE_H
