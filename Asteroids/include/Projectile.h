#define STRICT

#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#pragma once

#include "Entity.h"

class Projectile : public Entity
{
public:
	using Entity::Entity;

	// Gameplay
	int lifetime = 0;
};

#endif _PROJECTILE_H
