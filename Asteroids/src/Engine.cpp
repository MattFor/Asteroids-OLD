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

int Engine::add_entity(Entity::Type type, Entity::Owner owner = Entity::Owner::Unknown)
{
	switch (type)
	{
	case Entity::Type::Player:
	{
		Player* player = new Player(++this->object_count, ++this->entity_count, type, Entity::Owner::Engine);
		this->entities.push_back(player);
	}
	break;

	default:
	{
		Entity* entity = new Entity(++this->object_count, ++this->entity_count, type, owner);
		this->entities.push_back(entity);
	}
	}

	if (this->debug)
	{
		printf(("Added entity #" + std::to_string(this->entity_count) + "\n").c_str());
	}

	return (int)this->entities.size() - 1;
};

// Entity Logic

void Engine::calculate_moves()
{
	const float elapsed_time = this->get_elapsed_time();

	for (Entity* entity : this->entities)
	{
		entity->calc_move(elapsed_time);

		entity->x = (entity->x > this->screen_width) ? 0 : (entity->x < 0) ? this->screen_width : entity->x;
		entity->y = (entity->y > this->screen_height) ? 0 : (entity->y < 0) ? this->screen_height : entity->y;

		entity->spawn_cooldown = (entity->spawn_cooldown <= 0) ? 0 : --entity->spawn_cooldown;


		printf((std::to_string((int)entity->spawn)).c_str());

		if (entity->spawn != Entity::Type::Unknown)
		{
			const int index = this->add_entity(entity->spawn, entity->generate_ownership());

			if (entity->spawned_inheritance)
			{
				this->entities[index]->x = entity->x;
				this->entities[index]->y = entity->y;
			}

			if (this->debug)
			{
				printf(("Entity #" + std::to_string(entity->id) + " spawned entity #" + std::to_string(index)).c_str());
			}
		}

		//if (this->debug && entity->type == Entity::Type::Player)
		//{
		//	printf((
		//		"X: " + std::to_string(entity->x) +
		//		" Y: " + std::to_string(entity->y) +
		//		" vDx: " + std::to_string(entity->dx) +
		//		" vDy: " + std::to_string(entity->dy) +
		//		" Vel: " + std::to_string(entity->velocity) +
		//		" R: " + std::to_string(entity->angle) +
		//		" rR: " + std::to_string(entity->get_angle()) +
		//		'\n').c_str());
		//}
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
		printf(error.c_str());
		return LOADING_TEXTURE_FAILURE;
	}

	// Add player entity
	this->add_entity(Entity::Type::Player);

	// Set coordinates in the middle of the playable area
	this->entities[0]->x = (float)window->width / 2;
	this->entities[0]->y = (float)window->height / 2;

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
		printf(("[TEXTURE LOADED] - " + texture_name[i] + " - ./textures/" + texture_filenames[i] + '\n').c_str());
	}

	return "";
};