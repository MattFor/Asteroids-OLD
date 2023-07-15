#define STRICT

#ifndef _ASTEROID_H
#define _ASTEROID_H

#pragma once

#include "Window.h"
#include "Engine.h"


class Asteroid
{
public:
	Asteroid(int height = -1, int width = -1)
	{
		this->window = new Window(height, width);		// Automatically calculate window size if not specified.
	};

	~Asteroid()
	{
		delete window;
		delete engine;
	};


	int run();

	Window* window = 0;
	Engine* engine = new Engine();
};


#endif // !_ASTEROID_H
