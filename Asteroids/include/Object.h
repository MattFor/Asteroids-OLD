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

	struct Owner {
		SpawnableType type;
		int id;
	};

	Object() {};
	virtual ~Object()
	{
		delete shape;
		delete sprite;
		delete texture;
	};

	Owner owner;

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
	sf::Shape* shape = nullptr;

	virtual void set_shape(const float = 1.0f, sf::Color = sf::Color::Yellow);
	virtual void set_shape(std::vector<sf::Vector2f>, sf::Color = sf::Color::White, bool wireframe = true);

	// - TEXTURES Mode
	sf::Sprite* sprite = nullptr;
	sf::Texture* texture = nullptr;

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
