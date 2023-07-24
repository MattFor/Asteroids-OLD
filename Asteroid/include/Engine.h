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
	Engine() {};
	~Engine() {}; // Textures and entities already handled in their own deconstructor

	bool debug = true;

	// Start up
	int initialize(Window*);

	// Runtime
	void add_entity(Entity::Type);

	// Entity logic
	void execute_moves();
	void calculate_moves();

	// Graphics
	void apply_textures(Window*);
	void draw_all(sf::RenderWindow*);

	int key = sf::Keyboard::Unknown;

	int object_count = 0;
	int entity_count = 0;

	int asteroid_count = 0;
	int enemy_spaceship_count = 0;

	bool last_pressed_forward_flag = false;

	sf::Clock* timer = new sf::Clock();

	float get_elapsed_time();

	std::string load_textures();

	std::vector<Object*> objects {};
	std::vector<Entity*> entities {};

	std::vector<sf::Texture*> textures {};
};

#endif // !_ENGINE_H
