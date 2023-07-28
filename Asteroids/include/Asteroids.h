#define STRICT

#ifndef _ASTEROIDS_H
#define _ASTEROIDS_H

#pragma once

#include "Window.h"
#include "Engine.h"

class Asteroids
{
public:
	Asteroids()
	{
		// Automatically calculate window size if not specified.
		this->window = new Window();
		this->engine = new Engine(this->window->render_width, this->window->render_height);
	};
	~Asteroids() {};

	Window* window = nullptr;
	Engine* engine = nullptr;

	int initialize();
};

#endif // !_ASTEROIDS_H
