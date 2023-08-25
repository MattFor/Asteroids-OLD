#define STRICT

#include <filesystem>

#include "Asteroids.h"

int Asteroids::initialize()
{
	// Start the engine and load the textures.
	if (!this->engine->initialize(*window))
		return STARTUP_ERROR;

	if (DEBUG)
	{
		printf("Initialization successful.\n");
		printf((
			"Window Resolution: " +
			std::to_string(this->window->width) +
			"x" +
			std::to_string(this->window->height) +
			".\n" +
			"Engine Resolution: " +
			std::to_string(this->engine->render_width) +
			"x" +
			std::to_string(this->engine->render_height) +
			".\n"
			).c_str());
	}

	sf::Event event{};
	while (this->window->hwnd->isOpen())
	{
		while (this->window->hwnd->pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
				EXIT_GAME:
					this->window->hwnd->close();
					delete this->engine;
					return EXIT_SUCCESS;
			}
		}

		if (sf::Keyboard::isKeyPressed(EXIT))
		{
			goto EXIT_GAME;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
		{
			const int asteroid_id = this->engine->spawn(
				Element::Spawnable::Asteroid
			);

			this->engine->entities[asteroid_id]->x = (float)(rand() % this->window->render_width);
			this->engine->entities[asteroid_id]->y = (float)(rand() % this->window->render_height);

			this->engine->entities[asteroid_id]->set_shape(20.0f, sf::Color::Red, true);

			this->engine->entities[asteroid_id]->angle = std::clamp((float)(rand() % 10 + rand() % 360), 0.0f, 360.0f);

			this->engine->entities[asteroid_id]->x_mov_mod = 0.3f;
			this->engine->entities[asteroid_id]->y_mov_mod = 0.3f;
		}

		this->engine->handle_game_logic(*this->window);
	}

	return EXIT_SUCCESS;
};
