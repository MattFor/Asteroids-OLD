#define STRICT

#ifndef _WINDOW_H
#define _WINDOW_H

#pragma once

#include "SFML/Graphics.hpp"

#include "Element.h"

class Window
{
public:
	Window()
	{
		if (FULLSCREEN)
		{
			sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
			this->width = desktop.width;
			this->height = desktop.height;

			if (DEBUG)
			{
				printf(("Fullscreen mode enabled, resolution: " + std::to_string(this->width) + "x" + std::to_string(this->height) + '\n').c_str());
			}
		}
		else
		{
			this->width = WIDTH;
			this->height = HEIGHT;
		}

		if ((this->width <= 0 || this->height <= 0) && !FULLSCREEN)
		{
			this->width = 1024;
			this->height = 1024;

			if (DEBUG)
			{
				printf("Window - Using default engine display size 1024x1024.\n");
			}
		}

		this->render_width = this->width;
		this->render_height = this->height;

		if (OLD_SCHOOL)
		{
			if (DEBUG)
			{
				printf(("Old school mode enabled, new render resolution: " + std::to_string(this->render_width) + "x" + std::to_string(this->render_height) + '\n').c_str());
			}

			this->render_buffer = new sf::RenderTexture();
			if (!this->render_buffer->create(this->render_width, this->render_height))
			{
				printf("Critical error - could not spawn render texture!\n");
			}

			this->shader = new sf::Shader();
			if (!this->shader->loadFromFile("./resources/shaders/old_school.frag", sf::Shader::Fragment))
			{
				printf("Failed to load old_school.frag shader!\n");
			}

			// Set shader uniforms
			this->shader->setUniform("texture", sf::Shader::CurrentTexture);
			this->shader->setUniform("resolution", sf::Glsl::Vec2(
				(float)this->render_width, (float)this->render_height)
			);
		}

		this->hwnd = new sf::RenderWindow();
		this->hwnd->create(
			sf::VideoMode(this->width, this->height),
			"Asteroids",
			FULLSCREEN ? sf::Style::Fullscreen : sf::Style::Default
		);

		sf::View view = sf::View(
			sf::FloatRect(
				0.0f,
				0.0f,
				(float)this->render_width,
				(float)this->render_height
			)
		);

		this->hwnd->setView(view);
		this->hwnd->setVerticalSyncEnabled(true);
	};
	~Window()
	{
		delete hwnd;
		delete shader;
		delete render_buffer;
	};

	unsigned int width = 0;
	unsigned int height = 0;

	unsigned int render_width = 0;
	unsigned int render_height = 0;

	sf::Shader* shader = nullptr;
	sf::RenderWindow* hwnd = nullptr;
	sf::RenderTexture* render_buffer = nullptr;

	void clear();
	void display();
};

#endif // !_WINDOW_H
