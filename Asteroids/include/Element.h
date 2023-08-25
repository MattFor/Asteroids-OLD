#define STRICT

#ifndef _ELEMENT_H
#define _ELEMENT_H

#pragma once

#include "SFML/Graphics.hpp"

#include "Settings.h"

class Element
{
public:
	const enum class Spawnable : int
	{
		Player,
		Projectile,
		Asteroid,
		Enemy_Spaceship,
		UI_Element,
		Unknown
	};
	const enum class Spawner : int
	{
		Player,
		Projectile,
		Asteroid,
		Enemy_Spaceship,
		Engine,
		UI_Element,
		Unknown
	};
	const struct Owner
	{
		Spawner type;
		Element* element;
	};

	explicit Element(const int id, Spawnable type, Owner owner)
	{
		this->id = id;
		this->type = type;
		this->owner = owner;

		switch (type)
		{
			case Spawnable::Player:
			case Spawnable::Projectile:
			case Spawnable::Asteroid:
			case Spawnable::Enemy_Spaceship:
				this->_type = Type::Dynamic;
				break;
			case Spawnable::UI_Element:
			case Spawnable::Unknown:
				this->_type = Type::Static;
				break;
		}
	};
	virtual ~Element()
	{
		delete shape;
		delete sprite;
		delete texture;
	};

	// Logic
	int id;
	Owner owner;
	Spawnable type;

	// Location
	float x = 0.0f;
	float y = 0.0f;

	float angle = 0.0f;

	// Graphics
	// - VECTORS Mode
	sf::Shape* shape = nullptr;

	virtual void set_shape(const float = 1.0f, sf::Color = sf::Color::Yellow, bool wireframe = false, const float thickness = 5.0f / (OLD_SCHOOL ? 1.0f : 5.0f));
	virtual void set_shape(std::vector<sf::Vector2f>, sf::Color = sf::Color::White, bool wireframe = false, const float thickness = 5.0f / (OLD_SCHOOL ? 1.0f : 5.0f));

	// - TEXTURES Mode
	sf::Sprite* sprite = nullptr;
	sf::Texture* texture = nullptr;

	virtual void set_texture(sf::Texture&);

protected:
	const enum class Type : int
	{
		Static,
		Dynamic,
		Unknown
	};

	Type _type = Type::Unknown;

	void set_outline(sf::Shape&, sf::Color, bool wireframe, const float thickness);
};

#endif // !_ELEMENT_H
