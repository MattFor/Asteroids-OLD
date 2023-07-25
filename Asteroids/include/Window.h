#define STRICT

#ifndef _WINDOW_H
#define _WINDOW_H

#pragma once

#define RADIANS 3.14159f / 180.0f

#define DECAY 0.999f

#define MAX_SPEED 10000.0f
#define MAX_ROTATION 180.0f

#include "SFML/Graphics.hpp"

#define UP sf::Keyboard::W
#define DOWN sf::Keyboard::S
#define LEFT sf::Keyboard::A
#define RIGHT sf::Keyboard::D
#define SHOOT sf::Keyboard::E

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

		// Initialize the window
		sf::RenderWindow* window = new sf::RenderWindow();
		window->create(sf::VideoMode(height, width), "Asteroids");

		// Make it so that y starts at 0 from the bottom of the screen
		sf::View view = window->getDefaultView();
		view.setSize((float)this->width, -(float)this->height);
		window->setView(view);

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
