#define STRICT

#ifndef _WINDOW_H
#define _WINDOW_H

#pragma once

#include "SFML/Graphics.hpp"


class Window
{
public:
	Window(int height = -1, int width = -1) {
		this->m_height = (size_t)height;
		this->m_width = (size_t)width;

		if (height == -1) {
			this->m_height = (size_t)1024;
			this->m_width = (size_t)1024;
		}

		sf::RenderWindow* window = new sf::RenderWindow();

		window->create(sf::VideoMode((unsigned int)m_height, (unsigned int)m_width), "Asteroid");

		this->m_wnd = window;
		this->m_wnd->setVerticalSyncEnabled(true);
		//this->m_wnd->setFramerateLimit(144);
	};

	~Window()
	{
		delete m_wnd;
	};

	size_t m_height = 0;
	size_t m_width = 0;

	sf::RenderWindow* m_wnd = 0;
private:
	size_t d_height = 0;
	size_t d_width = 0;
};


#endif // !_WINDOW_H
