#define STRICT

#ifndef _ENTITY_H
#define _ENTITY_H

#pragma once

#include "SFML/Graphics.hpp"

#include "Element.h"

class Entity : public Element
{
public:
	using Element::Element;
	virtual ~Entity() {};

	// Movement
	float dx = 0.0f;
	float dy = 0.0f;

	float rotation = 0.0f;

	double x_mov_mod = 1.00000000000f;
	double y_mov_mod = 1.00000000000f;

	// Gameplay
	int spawn_cooldown = 0;

	bool spawned_inheritance = true;

	Spawnable spawn = Spawnable::Unknown;

	// Movement
	void move();
	void rotate();

	virtual void calc_move(float elapsed_time);
};

#endif // !_ENTITY_H
