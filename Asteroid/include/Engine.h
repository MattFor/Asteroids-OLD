#define STRICT

#ifndef _ENGINE_H
#define _ENGINE_H

#pragma once

#include <vector>

#include "SFML/Graphics.hpp"

#include "Window.h"
#include "Entity.h"


class Engine
{
public:
	Engine()
	{

	};

	~Engine() {};


	bool debug = true;

	void execute_moves();
	void add_entity(Entity* entity);
	void start_game(Window* window);
	void calculate_moves(Window* window);
	void draw_entities(sf::RenderWindow* window);
	
	int entity_ids = 0;
	int key_pressed = 0;

	bool last_pressed_forward_flag = false;

	std::string load_textures(Window* window);

	std::vector<Entity*> entities{};
	std::vector<sf::Texture> textures{};
};


#endif // !_ENGINE_H
