#define STRICT

#include "Object.h"

void Object::move()
{
	switch (RENDER_MODE)
	{
	case RenderMode::VECTORS:
		this->shape->setPosition(this->x, this->y);
		break;
	case RenderMode::TEXTURES:
		this->sprite->setPosition(this->x, this->y);
		break;
	}
};

void Object::rotate()
{
	switch (RENDER_MODE)
	{
	case RenderMode::VECTORS:
		this->shape->setRotation(this->angle);
		break;
	case RenderMode::TEXTURES:
		this->sprite->setRotation(this->angle);
		break;
	}
};

void Object::set_texture(sf::Texture& texture)
{
	sf::Sprite* sprite = new sf::Sprite(texture);

	sprite->setOrigin((sf::Vector2f)texture.getSize() / 2.0f);

	this->texture = &texture;

	this->sprite = sprite;
	this->sprite->setTexture(*this->texture);

	this->loaded = true;
};

void Object::set_shape(std::vector<sf::Vector2f> points, sf::Color colour, bool wireframe)
{
	size_t point_count = points.size();

	shape->setPointCount(point_count);

	for (int i = 0; i < point_count; i++)
	{
		shape->setPoint(i, points[i]);
	}

	shape->setOrigin(0, 0);
	shape->setPosition(sf::Vector2f(x, y));

	if (wireframe)
	{
		shape->setOutlineColor(colour);
		shape->setOutlineThickness(1.0f);
		shape->setFillColor(sf::Color::Transparent);
	}
	else
	{
		shape->setFillColor(colour);
	}
};
