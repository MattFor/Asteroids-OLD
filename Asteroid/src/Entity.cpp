#define STRICT

#include "Entity.h"

void Entity::calc_move(float elapsed_time)
{
	this->x += this->dx * elapsed_time;
	this->y += this->dy * elapsed_time;
};