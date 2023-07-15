#define STRICT

#ifndef _ENTITY_H
#define _ENTITY_H

#pragma once

#include "SFML/Graphics.hpp"

#include "Window.h"


class Entity
{
public:
	enum class Type : int
	{
		Player,
		Projectile,
		Asteroid_Big,
		Asteroid_Medium,
		Asteroid_Small,
		Asteroid_Special,
		Spaceship_Enemy,
		Unknown
	};

	Entity(Entity::Type type) {
		this->type = type;
	};

	~Entity() {};


	int id = 0;

	// x coord, y coord, velocity, direction;
	double x = 0.0f;
	double y = 0.0f;
	double v_dx = 0.0f;
	double v_dy = 0.0f;
	double velocity = 0.0f;
	double rotation = 0.0f;
	double saved_rotation = 0.0f;
	double rotation_change = 0.0f;

	int projectile_cooldown = 0;

	Type type = Type::Unknown;

	std::string owner = "";

	bool texture_loaded = false;

	sf::Sprite sprite;
	sf::Texture texture;

	void get_texture(sf::Texture texture, Window* window);
};


#endif // !_ENTITY_H
