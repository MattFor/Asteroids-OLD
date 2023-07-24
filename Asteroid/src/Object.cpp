#define STRICT

#include "Object.h"

void Object::move()
{
	this->sprite->setPosition(this->x, this->y);
};

void Object::rotate()
{
	this->sprite->setRotation(this->rotation);
};

void Object::get_rotation()
{
	this->sprite->getRotation();
};

void Object::set_texture(sf::Texture* texture)
{
	sf::Sprite* sprite = new sf::Sprite(*texture);

	sprite->setOrigin((sf::Vector2f)texture->getSize() / 2.0f);

	this->texture = texture;

	this->sprite = sprite;
	this->sprite->setTexture(*this->texture);

	this->loaded = true;
};