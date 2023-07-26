#define STRICT

#ifndef _WINDOW_H
#define _WINDOW_H

#pragma once

#define RADIANS 3.14159f / 180.0f

#define DECAY 0.985f

#define MAX_SPEED 10000.0f
#define MAX_ROTATION 180.0f

#include "SFML/Graphics.hpp"

#define EXIT sf::Keyboard::Escape

#define UP sf::Keyboard::W
#define DOWN sf::Keyboard::S
#define LEFT sf::Keyboard::A
#define RIGHT sf::Keyboard::D
#define SHOOT sf::Keyboard::E

enum class RenderMode : int
{
	VECTOR,
	TEXTURE
};

#define RENDER_MODE RenderMode::VECTOR

class Window
{
public:
	Window(const int width = -1, const int height = -1, bool fullscreen = false) {
		this->width = width;
		this->height = height;

		if (height == -1)
		{
			this->width = 1024;
			this->height = 1024;
		}
		else if (fullscreen)
		{
			sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
			this->width = desktop.width;
			this->height = desktop.height;
		}

		// Initialize the window
		sf::RenderWindow* window = new sf::RenderWindow();

		if (fullscreen)
		{
			window->create(sf::VideoMode(height, width), "Asteroids", sf::Style::Fullscreen);
		}
		else
		{
			window->create(sf::VideoMode(height, width), "Asteroids");
		}

		// Make it so that y starts at 0 from the bottom of the screen
		sf::View view = window->getDefaultView();
		view.setSize((float)this->width, -(float)this->height);
		window->setView(view);

		this->hwnd = window;
		this->hwnd->setVerticalSyncEnabled(true);
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
