#define STRICT

#ifndef _ASTEROIDS_H
#define _ASTEROIDS_H

#pragma once

#include "Window.h"
#include "Engine.h"

class Asteroids
{
public:
	Asteroids(int height = -1, int width = -1)
	{
		// Automatically calculate window size if not specified.
		this->window = new Window(height, width);
	};
	~Asteroids() {};

	Window* window = NULL;
	Engine* engine = new Engine();

	int initiate();
};

#endif // !_ASTEROIDS_H
