#define STRICT

#ifndef _ENGINE_H
#define _ENGINE_H

#pragma once

#include <vector>

#include "SFML/Graphics.hpp"

#include "Window.h"
#include "Element.h"
#include "Entity.h"
#include "Player.h"

class Engine
{
private:
	std::string element_names[7] = {
		"Player",
		"Projectile",
		"Asteroid",
		"Enemy_Spaceship",
		"Engine",
		"UI_Element",
		"Unknown"
	};

	void draw_objects(sf::RenderWindow& window, sf::RenderTexture& buffer);
	void draw_entities(sf::RenderWindow& window, sf::RenderTexture& buffer);

public:
	Engine(unsigned int render_width = 0, unsigned int render_height = 0)
	{
		if (render_width <= 0 || render_height <= 0)
		{
			this->render_width = 1024;
			this->render_height = 1024;

			if (DEBUG)
			{
				printf("Window - Using default engine display size 1024x1024.\n");
			}
		}
		else
		{
			this->render_width = render_width;
			this->render_height = render_height;
		}
	};
	~Engine() {}; // Textures and entities already handled in their own deconstructor

	// Internals
	unsigned int render_width = 0;
	unsigned int render_height = 0;

	sf::Clock* timer = new sf::Clock();

	// Logic
	int object_count = 0;
	std::vector<Element*> objects {};
	int entity_count = 0;
	std::vector<Entity*> entities {};

	// Graphics
	std::vector<sf::Texture*> textures {};

	// Start up
	int initialize(Window&);
	std::string load_textures();

	// Runtime
	float get_elapsed_time();

	int spawn(
		Element::Spawnable,	// Type
		Element::Owner = {
			Element::Spawner::Engine,
			nullptr
		} // Owner
	);

	void despawn(Element&);

	// Entity logic
	void execute_moves();
	void calculate_moves();

	// Graphics
	void apply_textures();
	void draw_all(sf::RenderWindow&, sf::RenderTexture&);
	void clear_screen(sf::RenderWindow&, sf::RenderTexture&);
};

#endif // !_ENGINE_H
