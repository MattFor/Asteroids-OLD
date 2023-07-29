#define STRICT

#include "Element.h"

// Graphics
void Element::set_texture(sf::Texture& texture)
{
	sf::Sprite* sprite = new sf::Sprite(texture);

	sprite->setOrigin(0.0f, 0.0f);

	this->texture = &texture;

	this->sprite = sprite;
	this->sprite->setTexture(*this->texture);
};

// Polymorphic implementation either makes it a circle or convex shape
void Element::set_shape(const float radius, sf::Color colour)
{
	this->shape = new sf::CircleShape(std::clamp(radius, 1.0f, 10000000.0f));
	sf::CircleShape* shape_cast = dynamic_cast<sf::CircleShape*>(this->shape);

	shape_cast->setOrigin(0.0f, 0.0f);
	shape_cast->setPosition(sf::Vector2f(x, y));

	shape_cast->setFillColor(colour);
};

void Element::set_shape(std::vector<sf::Vector2f> points, sf::Color colour, bool wireframe)
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
