#define STRICT

#include <vector>
#include <string>

#include "Window.h"
#include "Engine.h"

#include "Entity.h"
#include "Player.h"

// Runtime
float Engine::get_elapsed_time()
{
	return this->timer->restart().asSeconds();
};

void Engine::add_entity(Entity::Type type)
{
	switch (type)
	{
	case Entity::Type::Player:
	{
		Player* player = new Player(++this->object_count, ++this->entity_count, type);
		this->entities.push_back(player);
	}
	break;

	default:
	{
		Entity* entity = new Entity(++this->object_count, ++this->entity_count, type);
		this->entities.push_back(entity);
	}
	}

	if (this->debug)
	{
		std::printf(("Added entity #" + std::to_string(this->entity_count) + "\n").c_str());
	}
};

// Entity Logic

void Engine::calculate_moves()
{
	const float elapsed_time = this->get_elapsed_time();

	for (Entity* entity : this->entities)
	{
		entity->calc_move(elapsed_time);

		if (this->debug)
		{
			std::printf((
				"X: " + std::to_string(entity->x) +
				" Y: " + std::to_string(entity->y) +
				" vDx: " + std::to_string(entity->dx) +
				" vDy: " + std::to_string(entity->dy) +
				" Vel: " + std::to_string(entity->velocity) +
				" R: " + std::to_string(entity->rotation) +
				" Key: " + std::to_string(this->key) +
				'\n').c_str());
		}
	}
};

void Engine::execute_moves()
{
	for (auto entity : this->entities)
	{
		entity->move();
		entity->rotate();
	}
};

// Graphics

void Engine::apply_textures(Window* window)
{
	for (Entity* entity : this->entities) {
		if (entity->loaded)
			continue;

		entity->set_texture(this->textures[(int)entity->type]);
	}
};

void Engine::draw_all(sf::RenderWindow* window)
{
	this->draw_objects(window);
	this->draw_entities(window);

	window->display();
};

void Engine::draw_entities(sf::RenderWindow* window)
{
	for (Entity* entity : this->entities)
	{
		window->draw(*entity->sprite);
	}
};

void Engine::draw_objects(sf::RenderWindow* window)
{
	for (Object* object : this->objects)
	{
		window->draw(*object->sprite);
	}
};

// Start up

int Engine::initialize(Window* window)
{
	std::string error = this->load_textures();

	if (error != "")
	{
		std::printf(error.c_str());
		return LOADING_TEXTURE_FAILURE;
	}

	// Add player entity
	this->add_entity(Entity::Type::Player);

	// Set coordinates in the middle of the playable area
	this->entities[0]->x = (float)window->width / 2;
	this->entities[0]->y = (float)window->height / 2;

	// Move the player to the coordinates
	this->entities[0]->move();

	return STARTUP_SUCCESS;
};

std::string Engine::load_textures()
{
	std::string texture_filenames[2] = {
		"as_vec_player",
		"as_vec_projectile"
	};

	std::string texture_name[2] = {
		"PLAYER",
		"PROJECTILE"
	};

	for (int i = 0; i < 2; i++)
	{
		sf::Texture* texture = new sf::Texture();

		if (!texture->loadFromFile("./textures/" + texture_filenames[i] + ".png"))
		{
			delete texture; // Prevent memory leak
			return "Error loading " + texture_name[i] + ": " + texture_filenames[i] + ".png in /textures/\n";
		}

		this->textures.push_back(texture);
		std::printf(("[TEXTURE LOADED] - " + texture_name[i] + " - ./textures/" + texture_filenames[i] + '\n').c_str());
	}

	return "";
};