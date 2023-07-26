#define STRICT

#ifndef _ASTEROIDS_H
#define _ASTEROIDS_H

#pragma once

#include "Window.h"
#include "Engine.h"

class Asteroids
{
public:
	Asteroids(const int width = -1, int height = -1, bool fullscreen = false)
	{
		// Automatically calculate window size if not specified.
		this->window = new Window(width, height, fullscreen);
		this->engine = new Engine(this->window->width, this->window->height);
	};
	~Asteroids() {};

	Window* window = NULL;
	Engine* engine = NULL;

	int initiate();
};

#endif // !_ASTEROIDS_H
