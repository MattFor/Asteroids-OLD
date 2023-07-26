#define STRICT

#include <vector>
#include <string>

#include "Window.h"
#include "Engine.h"

// Runtime
float Engine::get_elapsed_time()
{
	return this->timer->restart().asSeconds();
};

int Engine::spawn(Object::Spawnable to_spawn, Object::Spawnable spawner)
{

	switch (to_spawn)
	{
	case Object::Spawnable::Player:
	{
		Player* player = new Player(
			++this->object_count, 
			++this->entity_count, 
			to_spawn, 
			// Using spawnable here to save creating a Spawner enum
			Object::Spawnable::Engine
		);
			
		this->entities.push_back(player);
	}
	break;

	default:
	{
		Entity* entity = new Entity(++this->object_count, ++this->entity_count, to_spawn, spawner);
		this->entities.push_back(entity);
	}
	}

	//if (this->debug)
	//{
	//	printf(("Added Object #" + std::to_string(this->object_count) + "\n").c_str());
	//}

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

		if (entity->spawn != Object::Spawnable::Unknown)
		{
			const int index = this->spawn(entity->spawn, entity->type);

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
		//		'\n').c_str());
		//}
	}
};

void Engine::execute_moves()
{
	for (auto entity : this->entities)
	{
		entity->move(this->render_mode);
		entity->rotate(this->render_mode);
	}
};

// Graphics
void Engine::apply_textures()
{
	for (Entity* entity : this->entities) {
		if (entity->loaded)
			continue;

		entity->set_texture(*this->textures[(int)entity->type]);
	}
};

void Engine::draw_all(sf::RenderWindow& window)
{
	this->draw_objects(window);
	this->draw_entities(window);

	window.display();
};

void Engine::draw_entities(sf::RenderWindow& window)
{
	if (render_mode == RenderMode::VECTOR)
	{
		for (Entity* entity : this->entities)
		{
			window.draw(*entity->shape);
		}

		return;
	}

	for (Entity* entity : this->entities)
	{
		window.draw(*entity->sprite);
	}
};

void Engine::draw_objects(sf::RenderWindow& window)
{
	if (render_mode == RenderMode::VECTOR)
	{
		for (Object* object : this->objects)
		{
			window.draw(*object->shape);
		}

		return;
	}

	for (Object* object : this->objects)
	{
		window.draw(*object->sprite);
	}
};

// Start up
int Engine::initialize(Window& window)
{
	if (render_mode == RenderMode::TEXTURE)
	{
		printf("Initializing in TEXTURE GRAPHICS mode.\n");

		std::string error = this->load_textures();

		if (error != "")
		{
			printf(error.c_str());
			return LOADING_TEXTURE_FAILURE;
		}
	} 
	else
	{
		printf("Initializing in VECTOR GRAPHICS mode.\n");
	}

	// Add player entity
	this->spawn(Object::Spawnable::Player, Object::Spawnable::Engine);

	// Set coordinates in the middle of the playable area
	this->entities[0]->x = (float)window.width / 2;
	this->entities[0]->y = (float)window.height / 2;

	if (render_mode == RenderMode::VECTOR)
	{
		std::vector<sf::Vector2f> points = {
			{ 0.0f, -20.0f },
			{ -10.0f, 20.0f },
			{ 10.0f, 20.0f }
		};

		this->entities[0]->set_shape(points, sf::Color::White, true);
	}

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