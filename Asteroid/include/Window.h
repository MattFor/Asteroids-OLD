#define STRICT

#ifndef _WINDOW_H
#define _WINDOW_H

#pragma once

#include "SFML/Graphics.hpp"

class Window
{
public:
	Window(int height = -1, int width = -1) {
		this->height = height;
		this->width = width;

		if (height == -1)
		{
			this->height = 1024;
			this->width = 1024;
		};

		sf::RenderWindow* window = new sf::RenderWindow();

		window->create(sf::VideoMode(height, width), "Asteroids");

		this->hwnd = window;
		this->hwnd->setVerticalSyncEnabled(true);
		// this->hwnd->setFramerateLimit(144);
	};
	~Window()
	{
		delete hwnd;
	};

	unsigned int width = 0;
	unsigned int height = 0;

	sf::RenderWindow* hwnd = NULL;

	void clear();
	void display();
};

#endif // !_WINDOW_H
