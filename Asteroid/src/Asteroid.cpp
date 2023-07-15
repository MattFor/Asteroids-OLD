#define STRICT

#include <filesystem>

#include "Asteroid.h"


int Asteroid::run()
{
	this->engine->start_game(this->window);
	std::string err_load = this->engine->load_textures(this->window);

	if (err_load != "") {
		std::printf(err_load.c_str());
		return 1;
	}

	sf::Event event{};

	while (this->window->m_wnd->isOpen())
	{
		while (this->window->m_wnd->pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					this->window->m_wnd->close();
					delete this->engine;
					return 0;

				case sf::Event::TextEntered:
					event.key;
			}
		}

		for (auto entity : this->engine->entities) {
			if (entity->texture_loaded)
				continue;

			entity->get_texture(this->engine->textures[(int)entity->type], this->window);
			entity->texture_loaded = true;
		}

		this->window->m_wnd->clear();
		this->engine->key_pressed = 0;

		this->engine->last_pressed_forward_flag = sf::Keyboard::isKeyPressed(sf::Keyboard::W);

		if (this->engine->last_pressed_forward_flag) {
			this->engine->entities[0]->velocity -= 0.02f;
			this->engine->entities[0]->saved_rotation = this->engine->entities[0]->sprite.getRotation();
			this->engine->key_pressed++;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			this->engine->entities[0]->rotation_change -= 0.1f;
			this->engine->key_pressed++;
		}
		
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		//	this->engine->entities[0]->velocity += 0.02f;
		//	this->engine->key_pressed = true;
		//}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			this->engine->entities[0]->rotation_change += 0.1f;
			this->engine->key_pressed++;
		}

		this->engine->calculate_moves(this->window);

		this->engine->execute_moves();

		this->engine->draw_entities(this->window->m_wnd);

		this->window->m_wnd->display();
	}

	return 0;
};
