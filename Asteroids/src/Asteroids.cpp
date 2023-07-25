#define STRICT

#include <filesystem>

#include "Asteroids.h"

int Asteroids::initiate()
{
	// Start the engine and load the textures.
	if (!this->engine->initialize(this->window))
		return STARTUP_ERROR;

	sf::Event event {};

	while (this->window->hwnd->isOpen())
	{
		while (this->window->hwnd->pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				this->window->hwnd->close();
				delete this->engine;
				return EXIT_SUCCESS;
			}
		}

		// Clear the window and apply the textures again,
		// set their correct positions
		this->window->clear();
		this->engine->apply_textures(this->window);

		this->engine->calculate_moves();
		this->engine->execute_moves();

		// Display everything
		this->engine->draw_all(this->window->hwnd);
		this->window->display();
	}

	return EXIT_SUCCESS;
};