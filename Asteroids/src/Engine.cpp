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

int Engine::spawn(Element::Spawnable to_spawn, Element::Owner owner)
{
	switch (to_spawn)
	{
	case Element::Spawnable::Player:
	{
		Player* player = new Player(
			++this->entity_count,
			to_spawn,
			// Using spawnable here to save creating a Spawner enum
			Element::Owner{ Element::Spawner::Engine, nullptr }
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
			printf("Player spawn coordinates: X[%.2f] Y[%.2f]\n", player->x, player->y);
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
	}
	break;

	case Element::Spawnable::Projectile:
	{
		Entity* projectile = new Entity(++this->entity_count, to_spawn, owner);

		if (RENDER_MODE == RenderMode::VECTORS)
		{
			const float scale = (float)render_height / 1024.0f;
			projectile->set_shape(3.0f * scale, sf::Color::Yellow);
		}

		this->entities.push_back(projectile);
	}
	break;

	default:
	{
		Entity* entity = new Entity(++this->entity_count, to_spawn, owner);
		this->entities.push_back(entity);
	}
	}

	if (DEBUG)
	{
		printf("Added Entity #%d %s from %s\n", this->entity_count, element_names[(int)to_spawn].c_str(), element_names[(int)owner.type].c_str());
	}

	return (int)this->entities.size() - 1;
};

void Engine::despawn(Element& element)
{
	if (Entity* entity = dynamic_cast<Entity*>(&element))
	{
		// Log the despawn action.
		printf(("Despawning Entity " + element_names[(int)entity->type] + " ID #" + std::to_string(entity->id) + '\n').c_str());
	}

	entities.erase(std::remove(entities.begin(), entities.end(), &element), entities.end());
}

// Entity Logic
void Engine::calculate_moves()
{
	const float elapsed_time = get_elapsed_time();

	std::vector<Entity*> marked_for_deletion {};
	std::vector<std::tuple<Element::Spawnable, Element::Owner>> spawns {};

	for (auto it = entities.begin(); it != entities.end();)
	{
		Entity* entity = *it;

		entity->calc_move(elapsed_time);

		const bool OUT_OF_BOUNDS = entity->x > render_width || entity->x < 0 || entity->y > render_height || entity->y < 0;
		if (OUT_OF_BOUNDS && entity->type == Element::Spawnable::Projectile)
		{
			marked_for_deletion.push_back(entity);
		}

		entity->x = (entity->x > render_width) ? 0 : (entity->x < 0) ? render_width : entity->x;
		entity->y = (entity->y > render_height) ? 0 : (entity->y < 0) ? render_height : entity->y;
		entity->spawn_cooldown = std::max(entity->spawn_cooldown - 1, 0);

		if (entity->spawn != Element::Spawnable::Unknown)
		{
			spawns.push_back({ entity->spawn, Element::Owner{ (Element::Spawner)entity->type, entity } });
		}

		if (DEBUG_PLAYER_INFO && entity->type == Element::Spawnable::Player)
		{
			printf("X[%.3f] Y[%.3f] A[%.3f] DX[%.3f] DY[%.3f] R[%.3f]\n", entity->x, entity->y, entity->angle, entity->dx, entity->dy, entity->rotation);
		}

		++it;
	}

	// Spawn entities to prevent modifying entities while going through a vector
	for (auto& [to_spawn, owner] : spawns)
	{
		const int index = spawn(to_spawn, owner);

		Entity* entity = dynamic_cast<Entity*>(owner.element);
		if (entity->spawned_inheritance)
		{
			sf::FloatRect entity_bounds {};
			if (entity->shape != nullptr)
			{
				entity_bounds = entity->shape->getLocalBounds();
			}
			else if (entity->sprite != nullptr)
			{
				entity_bounds = entity->sprite->getLocalBounds();
			}

			const float y_offset = entity_bounds.height / 2.0f;

			sf::FloatRect spawned_entity_bounds {};
			if (entities[index]->shape != nullptr)
			{
				spawned_entity_bounds = entities[index]->shape->getLocalBounds();
			}
			else if (entities[index]->sprite != nullptr)
			{
				spawned_entity_bounds = entities[index]->sprite->getLocalBounds();
			}

			const float x_offset = spawned_entity_bounds.width / 2.0f;

			const float angle_radians = entity->angle * RADIANS;
			const float x_offset_rotated = y_offset * std::sin(angle_radians) - x_offset;
			const float y_offset_rotated = y_offset * -std::cos(angle_radians);

			entities[index]->dx = entity->dx;
			entities[index]->dy = entity->dy;
			entities[index]->x = entity->x + x_offset_rotated;
			entities[index]->y = entity->y + y_offset_rotated;
			entities[index]->angle = entity->angle;
		}

		entity->spawn = Element::Spawnable::Unknown;
	}

	// Deleting them here to prevent the same thing as well
	for (Entity* entity : marked_for_deletion)
	{
		despawn(*entity);
	}

	// Adjust the IDs
	for (int i = 0; i < entities.size(); ++i)
	{
		entities[i]->id = i;
	}
};

void Engine::execute_moves()
{
	for (Entity* entity : entities)
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
	for (Entity* entity : entities)
	{
		entity->set_texture(*textures[(int)entity->type]);
	}
};

void Engine::draw_all(sf::RenderWindow& window, sf::RenderTexture& buffer)
{
	draw_objects(window, buffer);
	draw_entities(window, buffer);

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
		if (OLD_SCHOOL)
		{
			for (Entity* entity : entities)
			{
				buffer.draw(*entity->shape);
			}
		}
		else
		{
			for (Entity* entity : entities)
			{
				window.draw(*entity->shape);
			}
		}
		break;

	case RenderMode::TEXTURES:
		for (Entity* entity : entities)
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
		if (OLD_SCHOOL)
		{
			for (Element* object : objects)
			{
				buffer.draw(*object->shape);
			}
		}
		else
		{
			for (Element* object : objects)
			{
				window.draw(*object->shape);
			}
		}
		break;

	case RenderMode::TEXTURES:
		for (Element* object : objects)
		{
			window.draw(*object->sprite);
		}
		break;
	}
};

// Start up
int Engine::initialize(Window& window)
{
	if (OLD_SCHOOL && RENDER_MODE != RenderMode::VECTORS)
	{
		printf("Cannot play in old school mode without being in the VECTOR graphics mode.\n");
		return STARTUP_ERROR;
	}

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

		std::string error = load_textures();

		if (error != "")
		{
			printf(error.c_str());
			return LOADING_TEXTURE_FAILURE;
		}
		break;
	}

	// Add player entity
	spawn(Element::Spawnable::Player, Element::Owner{ Element::Spawner::Engine, nullptr });

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

		textures.push_back(texture);

		if (DEBUG)
		{
			printf("[TEXTURES LOADED] - %s - ./textures/%s\n", texture_name[i].c_str(), texture_filenames[i].c_str());
		}
	}

	return "";
};
