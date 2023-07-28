#define STRICT

#include "Object.h"

void Object::move()
{
	switch (RENDER_MODE)
	{
	case RenderMode::VECTORS:
		if (this->shape == nullptr)
			return;
		this->shape->setPosition(this->x, this->y);
		break;
	case RenderMode::TEXTURES:
		if (this->sprite == nullptr)
			return;
		this->sprite->setPosition(this->x, this->y);
		break;
	}
};

void Object::rotate()
{
	switch (RENDER_MODE)
	{
	case RenderMode::VECTORS:
		if (this->shape == nullptr)
			return;
		this->shape->setRotation(this->angle);
		break;
	case RenderMode::TEXTURES:
		if (this->sprite == nullptr)
			return;
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

void Object::set_shape(const float radius, sf::Color colour)
{
	this->shape = new sf::CircleShape(std::clamp(3.0f * (OLD_SCHOOL ? OLD_SCHOOL_FACTOR : 1), 1.0f, 10000000.0f));
	sf::CircleShape* shape_cast = dynamic_cast<sf::CircleShape*>(this->shape);

	shape_cast->setOrigin(0.0f, 0.0f);
	shape_cast->setPosition(sf::Vector2f(x, y));

	shape_cast->setFillColor(colour);
};

void Object::set_shape(std::vector<sf::Vector2f> points, sf::Color colour, bool wireframe)
{
	const int point_count = (int)points.size();

	this->shape = new sf::ConvexShape();
	sf::ConvexShape* shape_cast = dynamic_cast<sf::ConvexShape*>(shape);

	shape_cast->setPointCount(point_count);
	for (int i = 0; i < point_count; i++)
	{
		shape_cast->setPoint(i, points[i]);
	}

	shape_cast->setOrigin(0.0f, 0.0f);
	shape_cast->setPosition(sf::Vector2f(x, y));

	if (wireframe)
	{
		shape_cast->setOutlineColor(colour);
		shape_cast->setOutlineThickness(1.0f);
		shape_cast->setFillColor(sf::Color::Transparent);
	}
	else
	{
		shape_cast->setFillColor(colour);
	}
};
