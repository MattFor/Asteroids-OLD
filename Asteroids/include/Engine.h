#define STRICT

#ifndef _ENGINE_H
#define _ENGINE_H

#pragma once

#include <vector>

#include "SFML/Graphics.hpp"

#include "Window.h"
#include "Object.h"
#include "Entity.h"
#include "Player.h"
#include "Projectile.h"

class Engine
{
private:
	void draw_objects(sf::RenderWindow&, sf::RenderTexture&);
	void draw_entities(sf::RenderWindow&, sf::RenderTexture&);

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

	int object_count = 0;
	int entity_count = 0;

	int asteroid_count = 0;
	int enemy_spaceship_count = 0;

	sf::Clock* timer = new sf::Clock();

	// Elements
	std::vector<Object*> objects {};
	std::vector<Entity*> entities {};

	// Graphics
	std::vector<sf::Texture*> textures {};

	// Start up
	int initialize(Window&);

	std::string load_textures();

	// Runtime
	float get_elapsed_time();

	int spawn(
		Object::SpawnableType,	// Type
		Object::SpawnableType	// Owner
	);

	// Entity logic
	void execute_moves();
	void calculate_moves();

	// Graphics
	void apply_textures();
	void draw_all(sf::RenderWindow&, sf::RenderTexture&);
	void clear_screen(sf::RenderWindow&, sf::RenderTexture&);
};

#endif // !_ENGINE_H
