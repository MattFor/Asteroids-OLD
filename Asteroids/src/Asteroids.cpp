#define STRICT

#include <filesystem>

#include "Asteroids.h"

int Asteroids::initialize()
{
	// Start the engine and load the textures.
	if (!this->engine->initialize(*this->window))
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

	sf::Event event {};
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

		// Clear the window and apply the textures again,
		// set their correct positions
		this->engine->clear_screen(*this->window->hwnd, *this->window->render_buffer);

		if (RENDER_MODE == RenderMode::TEXTURES)
		{
			this->engine->apply_textures();
		}

		this->engine->calculate_moves();
		this->engine->execute_moves();

		// Display everything
		this->engine->draw_all(*this->window->hwnd, *this->window->render_buffer);
	}

	return EXIT_SUCCESS;
};
