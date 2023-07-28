#define STRICT

#ifndef _OBJECT_H
#define _OBJECT_H

#pragma once

#include "SFML/Graphics.hpp"
#include "Window.h"

class Object
{
public:
	enum class ElementType : int {
		Static,
		Dynamic,
		Unknown
	};

	enum class SpawnableType : int
	{
		Player,
		Projectile,
		Asteroid,
		Enemy_Spaceship,
		Engine,
		Unknown
	};

	Object()
	{
		shape->setOrigin(0.0f, 0.0f);
		shape->setPosition(sf::Vector2f(x, y));
	};
	virtual ~Object()
	{
		delete shape;
		delete sprite;
		delete texture;
	};

	// Physics
	float size = 0.0f;
	float angle = 0.0f;

	float x = 0.0f;
	float y = 0.0f;

	float dx = 0.0f;
	float dy = 0.0f;

	float rotation = 0.0f;
	float velocity = 0.0f;

	// Graphics
	// - VECTORS Mode
	sf::ConvexShape* shape = new sf::ConvexShape();

	virtual void set_shape(std::vector<sf::Vector2f>, sf::Color = sf::Color::White, bool wireframe = true);

	// - TEXTURES Mode
	sf::Sprite* sprite = new sf::Sprite();
	sf::Texture* texture = new sf::Texture();

	bool loaded = false;

	virtual void set_texture(sf::Texture&);

	// Movement
	void move();
	void rotate();

protected:
	int _id = 0;
	ElementType _type = ElementType::Unknown;
};

#endif // !_OBJECT_H
