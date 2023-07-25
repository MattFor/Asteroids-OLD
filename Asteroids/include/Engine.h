#define STRICT

#ifndef _ENGINE_H
#define _ENGINE_H

#pragma once

#define STARTUP_ERROR 0
#define STARTUP_SUCCESS 1

#define LOADING_TEXTURE_FAILURE 0
#define LOADING_TEXTURE_SUCCESS 1

#include <vector>

#include "SFML/Graphics.hpp"

#include "Window.h"
#include "Entity.h"

class Engine
{
private:
	void draw_objects(sf::RenderWindow* window);
	void draw_entities(sf::RenderWindow* window);

public:
	Engine(const int screen_width = 1024, const int screen_height = 1024)
	{
		this->screen_width = screen_width;
		this->screen_height = screen_height;
	};
	~Engine() {}; // Textures and entities already handled in their own deconstructor

	bool debug = true;

	int screen_width = 1024;
	int screen_height = 1024;

	int object_count = 0;
	int entity_count = 0;

	int asteroid_count = 0;
	int enemy_spaceship_count = 0;

	bool last_pressed_forward_flag = false;

	sf::Clock* timer = new sf::Clock();

	std::vector<Object*> objects {};
	std::vector<Entity*> entities {};

	std::vector<sf::Texture*> textures {};

	// Start up
	int initialize(Window*);

	std::string load_textures();

	// Runtime
	float get_elapsed_time();

	int add_entity(Entity::Type, Entity::Owner);

	// Entity logic
	void execute_moves();
	void calculate_moves();

	// Graphics
	void apply_textures(Window*);
	void draw_all(sf::RenderWindow*);
};

#endif // !_ENGINE_H
