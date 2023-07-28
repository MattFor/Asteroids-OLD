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
			projectile->set_shape(3.0f, sf::Color::Yellow);
		}

		this->entities.push_back(projectile);

		if (DEBUG)
		{
			printf(("Added Entity #" + std::to_string(this->entity_count) + " Type: " + std::to_string((int)to_spawn) + "\n").c_str());
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

void Engine::despawn(Object& object)
{
	std::string object_name = "";

	if (Entity* entity = dynamic_cast<Entity*>(&object))
	{
		switch (entity->type)
		{
		case Object::SpawnableType::Player:
			object_name = "Player";
			break;
		case Object::SpawnableType::Projectile:
			object_name = "Projectile";
			break;
		case Object::SpawnableType::Asteroid:
			object_name = "Asteroid";
			break;
		case Object::SpawnableType::Enemy_Spaceship:
			object_name = "Enemy_Spaceship";
			break;
		case Object::SpawnableType::Engine:
			object_name = "Engine";
			break;
		default:
			object_name = "Unknown";
		}

		// Log the despawn action.
		printf(("Despawning Entity " + object_name + " ID #" + std::to_string(entity->id) + '\n').c_str());
	}

	entities.erase(std::remove(entities.begin(), entities.end(), &object), entities.end());
}

// Entity Logic
void Engine::calculate_moves()
{
	const float elapsed_time = this->get_elapsed_time();

	std::vector<int> marked_for_deletion {};
	std::vector<std::tuple<Object::SpawnableType, Object::SpawnableType, Entity*>> spawns {};

	for (auto it = this->entities.begin(); it != this->entities.end();)
	{
		Entity* entity = *it;

		entity->calc_move(elapsed_time);

		const bool OUT_OF_BOUNDS = entity->x > render_width || entity->x < 0 || entity->y > render_height || entity->y < 0;

		if (OUT_OF_BOUNDS && entity->type == Object::SpawnableType::Projectile)
		{
			marked_for_deletion.push_back(entity->id);
		}

		entity->x = (entity->x > render_width) ? 0 : (entity->x < 0) ? render_width : entity->x;
		entity->y = (entity->y > render_height) ? 0 : (entity->y < 0) ? render_height : entity->y;

		entity->spawn_cooldown = (entity->spawn_cooldown <= 0) ? 0 : --entity->spawn_cooldown;

		if (entity->spawn != Object::SpawnableType::Unknown)
		{
			spawns.push_back({ entity->spawn, entity->type, entity });
			entity->spawn = Object::SpawnableType::Unknown;
		}

		//if (Projectile* projectile = dynamic_cast<Projectile*>(entity))
		//{
		//	projectile->lifetime--;

		//	if (projectile->lifetime <= 0)
		//	{
		//		delete entity;
		//		it = this->entities.erase(it);
		//		continue;
		//	}
		//}

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

	for (auto& [to_spawn, owner, entity] : spawns) {
		const int index = this->spawn(to_spawn, owner);

		if (entity->spawn_inheritance)
		{
			this->entities[index]->x = entity->x;
			this->entities[index]->y = entity->y;
			this->entities[index]->angle = entity->angle;
		}

		switch (to_spawn)
		{
		case Object::SpawnableType::Projectile:
		{
			Projectile* projectile = dynamic_cast<Projectile*>(this->entities[index]);

			projectile->lifetime = 60 * 60 * 1000;
			projectile->set_shape();
		}
		break;
		}
	}

	for (int index : marked_for_deletion)
	{
		despawn(*this->entities[index]);
	}

	// Adjust the IDs
	for (int i = 0; i < this->entities.size(); ++i)
	{
		this->entities[i]->id = i;
	}
};

void Engine::execute_moves()
{
	for (Entity* entity : this->entities)
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

	// Apply filter
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
			if (entity->shape == nullptr)
				continue;

			if (OLD_SCHOOL)
			{
				buffer.draw(*entity->shape);
				break;
			}

			window.draw(*entity->shape);
		}
		break;

	case RenderMode::TEXTURES:
		for (Entity* entity : this->entities)
		{
			if (entity->sprite == nullptr)
				continue;

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
			if (object->shape == nullptr)
				continue;

			if (OLD_SCHOOL)
			{
				buffer.draw(*object->shape);
				break;
			}

			window.draw(*object->shape);
		}
		break;

	case RenderMode::TEXTURES:
		for (Object* object : this->objects)
		{
			if (object->sprite == nullptr)
				continue;

			window.draw(*object->sprite);
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
	this->entities[0]->spawn_inheritance = true;

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
