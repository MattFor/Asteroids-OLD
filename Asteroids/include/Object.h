#define STRICT

#ifndef _OBJECT_H
#define _OBJECT_H

#pragma once

#include "SFML/Graphics.hpp"
#include "Window.h"

class Object
{
public:
	enum class Type : int {
		Static,
		Dynamic,
		Entity,
		Unknown
	};

	~Object()
	{
		delete sprite;
		delete texture;
	};

	float angle = 0.0f;

	float x = 0.0f;
	float y = 0.0f;

	float dx = 0.0f;
	float dy = 0.0f;

	float rotation = 0.0f;
	float velocity = 0.0f;

	sf::Sprite* sprite = new sf::Sprite();
	sf::Texture* texture = new sf::Texture();

	bool loaded = false;

	void move();
	void rotate();

	float get_angle();

	virtual void set_texture(sf::Texture*);

protected:
	int _id = 0;

	Type _type = Type::Unknown;
};

#endif // !_OBJECT_H
