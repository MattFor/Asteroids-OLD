#define STRICT

#include "Projectile.h"

void Projectile::set_shape(const float radius, sf::Color colour)
{
	this->shape = new sf::CircleShape(radius);

	shape->setOrigin(0, 0);
	shape->setFillColor(colour);
	shape->setPosition(sf::Vector2f(x, y));
};
