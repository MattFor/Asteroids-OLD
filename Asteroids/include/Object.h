#define STRICT

#ifndef _OBJECT_H
#define _OBJECT_H

#pragma once

#include "SFML/Graphics.hpp"
#include "Element.h"

class Object : public Element
{
public:
	Object() {};
	~Object() {};

	// Movement
	float dx = 0.0f;
	float dy = 0.0f;

	float rotation = 0.0f;
	float velocity = 0.0f;

	float size = 0.0f;

	// Movement
	void move();
	void rotate();
};

#endif // !_OBJECT_H
