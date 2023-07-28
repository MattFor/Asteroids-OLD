#define STRICT

#include <string>
#include <vector>

#include "Window.h"
#include "Engine.h"

// Runtime
float Engine::get_elapsed_time()
{
	return this->timer->restart().asSeconds();
};

int Engine::spawn(Object::SpawnableType to_spawn, Object::SpawnableType spawner)
{
	switch (to_spawn)
	{
	case Object::SpawnableType::Player:
	{
		Player* player = new Player(
			++this->object_count,
			++this->entity_count,
			to_spawn,
			// Using spawnable here to save creating a Spawner enum
			Object::SpawnableType::Engine
		);

		// Set coordinates in the middle of the playable area
		player->x = (float)render_width / 2;
		player->y = (float)render_height / 2;

		// Make it upright
		if (OLD_SCHOOL)
		{
			player->angle = 180.0f;
		}

		if (DEBUG)
		{
			printf(("Player spawn coordinates: X[" + std::to_string(player->x) + "] Y[" + std::to_string(player->y) + "]\n").c_str());
		}

		if (RENDER_MODE == RenderMode::VECTORS)
		{
			const float scale = (float)render_height / 1024.0f;

			std::vector<sf::Vector2f> points = {
				{ 0.0f * scale, -20.0f * scale },
				{ -10.0f * scale, 20.0f * scale },
				{ 10.0f * scale, 20.0f * scale }
			};

			player->set_shape(points, sf::Color::White, true);
		}

		this->entities.push_back(player);

		if (DEBUG)
		{
			printf(("Added Entity #" + std::to_string(this->entity_count) + "\n").c_str());
		}
	}
	break;

	case Object::SpawnableType::Projectile:
	{
		Projectile* projectile = new Projectile(++this->object_count, ++this->entity_count, to_spawn, spawner);

		if (RENDER_MODE == RenderMode::VECTORS)
		{
			const float scale = (float)render_height / 1024.0f;
			projectile->set_shape(1.0f * scale, sf::Color::Yellow);
		}

		this->entities.push_back(projectile);

		if (DEBUG)
		{
			printf(("Added Entity #" + std::to_string(this->entity_count) + "\n").c_str());
		}
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

	for (auto it = this->entities.begin(); it != this->entities.end();)
	{
		Entity* entity = *it;

		entity->calc_move(elapsed_time);

		entity->x = (entity->x > render_width) ? 0 : (entity->x < 0) ? render_width : entity->x;
		entity->y = (entity->y > render_height) ? 0 : (entity->y < 0) ? render_height : entity->y;

		entity->spawn_cooldown = (entity->spawn_cooldown <= 0) ? 0 : --entity->spawn_cooldown;

		if (entity->spawn != Object::SpawnableType::Unknown)
		{
			const int index = this->spawn(entity->spawn, entity->type);

			if (entity->spawned_inheritance)
			{
				this->entities[index]->x = entity->x;
				this->entities[index]->y = entity->y;
				this->entities[index]->angle = entity->angle;
			}

			if (DEBUG)
			{
				printf(("Entity #" + std::to_string(entity->id) + " spawned entity #" + std::to_string(index) + '\n').c_str());
			}
		}

		if (Projectile* projectile = dynamic_cast<Projectile*>(entity))
		{
			projectile->lifetime--;

			if (projectile->lifetime <= 0)
			{
				delete entity;
				it = this->entities.erase(it);
				continue;
			}
		}

		if (DEBUG_PLAYER_INFO && entity->type == Object::SpawnableType::Player)
		{
			printf((
				"X: " + std::to_string(entity->x) +
				" Y: " + std::to_string(entity->y) +
				" vDx: " + std::to_string(entity->dx) +
				" vDy: " + std::to_string(entity->dy) +
				" Vel: " + std::to_string(entity->velocity) +
				" R: " + std::to_string(entity->angle) +
				'\n').c_str());
		}

		++it;
	}

	// Adjust the IDs
	for (int i = 0; i < this->entities.size(); ++i)
	{
		this->entities[i]->id = i;
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
void Engine::clear_screen(sf::RenderWindow& window, sf::RenderTexture& buffer)
{
	if (OLD_SCHOOL)
	{
		buffer.clear();
	}

	window.clear();
};

void Engine::apply_textures()
{
	for (Entity* entity : this->entities) {
		if (entity->loaded)
			continue;

		entity->set_texture(*this->textures[(int)entity->type]);
	}
};

void Engine::draw_all(sf::RenderWindow& window, sf::RenderTexture& buffer)
{
	this->draw_objects(window, buffer);
	this->draw_entities(window, buffer);

	if (OLD_SCHOOL)
	{
		sf::Sprite sprite;
		sprite.setTexture(buffer.getTexture());
		window.draw(sprite);
	}

	window.display();
};

void Engine::draw_entities(sf::RenderWindow& window, sf::RenderTexture& buffer)
{
	switch (RENDER_MODE)
	{
	case RenderMode::VECTORS:
		for (Entity* entity : this->entities)
		{
			if (OLD_SCHOOL)
			{
				buffer.draw(*entity->shape);
				continue;
			}

			window.draw(*entity->shape);
		}
		break;

	case RenderMode::TEXTURES:
		for (Entity* entity : this->entities)
		{
			window.draw(*entity->sprite);
		}
		break;
	}
};

void Engine::draw_objects(sf::RenderWindow& window, sf::RenderTexture& buffer)
{
	switch (RENDER_MODE)
	{
	case RenderMode::VECTORS:
		for (Object* object : this->objects)
		{
			if (OLD_SCHOOL)
			{
				buffer.draw(*object->shape);
				continue;
			}

			window.draw(*object->shape);
		}
		break;

	case RenderMode::TEXTURES:
		for (Object* object : this->objects)
		{
			buffer.draw(*object->sprite);
		}
		break;
	}
};

// Start up
int Engine::initialize(Window& window)
{
	switch (RENDER_MODE)
	{
	case RenderMode::VECTORS:
		if (DEBUG)
		{
			printf("Initializing in VECTOR GRAPHICS mode.\n");
		}
		break;

	case RenderMode::TEXTURES:
		if (DEBUG)
		{
			printf("Initializing in TEXTURE GRAPHICS mode.\n");
		}

		std::string error = this->load_textures();

		if (error != "")
		{
			printf(error.c_str());
			return LOADING_TEXTURE_FAILURE;
		}
		break;
	}

	// Add player entity
	this->spawn(Object::SpawnableType::Player, Object::SpawnableType::Engine);

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

		if (DEBUG)
		{
			printf(("[TEXTURES LOADED] - " + texture_name[i] + " - ./textures/" + texture_filenames[i] + '\n').c_str());
		}
	}

	return "";
};
