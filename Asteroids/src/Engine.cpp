#define STRICT

#include <string>
#include <vector>

#include "Window.h"
#include "Engine.h"

// Runtime
void Engine::handle_game_logic(Window& window)
{
	this->clear_screen(*window.hwnd, *window.render_buffer);

	switch (RENDER_MODE)
	{
		case RenderMode::TEXTURES:
		{
			this->apply_textures();
			break;
		}
	}

	this->process_entites();
	this->process_spawns();
	this->process_despawns();
	this->execute_moves();

	this->display_all(*window.hwnd, *window.render_buffer, *window.shader);
};

int Engine::spawn(Element::Spawnable to_spawn, Element::Owner owner)
{
	switch (to_spawn)
	{
		case Element::Spawnable::Player:
		{
			Player* player = new Player(
				++entity_count,
				to_spawn,
				// Using spawnable here to save creating a Spawner enum
				Element::Owner{ Element::Spawner::Engine, nullptr }
			);

			// Set coordinates in the middle of the playable area
			player->x = (float)render_width / 2;
			player->y = (float)render_height / 2;

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

			entities.push_back(player);
		}
		break;

		case Element::Spawnable::Projectile:
		{
			Entity* projectile = new Entity(++entity_count, to_spawn, owner);

			if (RENDER_MODE == RenderMode::VECTORS)
			{
				const float scale = (float)render_height / 1024.0f;
				projectile->set_shape(3.0f * scale, sf::Color::Yellow);
			}

			entities.push_back(projectile);
		}
		break;

		default:
		{
			Entity* entity = new Entity(++entity_count, to_spawn, owner);
			entities.push_back(entity);
		}
	}

	if (DEBUG)
	{
		printf("Added Entity #%d %s from %s\n", entity_count, element_names[(int)to_spawn].c_str(), element_names[(int)owner.type].c_str());
	}

	return (int)entities.size() - 1;
};

void Engine::despawn(Element& element)
{
	if (Entity* entity = dynamic_cast<Entity*>(&element))
	{
		// Log the despawn action.
		printf(("Despawning Entity " + element_names[(int)entity->type] + " ID #" + std::to_string(entity->id) + '\n').c_str());
	}

	entities.erase(std::remove(entities.begin(), entities.end(), &element), entities.end());
};

float Engine::get_elapsed_time()
{
	return timer->restart().asSeconds();
};

// Logic
void Engine::process_entites()
{
	const float elapsed_time = this->get_elapsed_time();

	for (auto it = this->entities.begin(); it != this->entities.end();)
	{
		Entity* entity = *it;

		entity->calc_move(elapsed_time);

		if (this->process_collisions(*entity))
		{
			entity->x = (entity->x > this->render_width) ? 0 : (entity->x < 0) ? this->render_width : entity->x;
			entity->y = (entity->y > this->render_height) ? 0 : (entity->y < 0) ? this->render_height : entity->y;

			entity->spawn_cooldown = std::max(entity->spawn_cooldown - 1, 0);
			if (entity->spawn != Element::Spawnable::Unknown)
			{
				this->to_spawn.push_back({ entity->spawn, Element::Owner{ (Element::Spawner)entity->type, entity } });
			}
		}

		++it;
	}
};

bool Engine::process_collisions(Element& element)
{
	bool marked = false;

	if (Player* entity = dynamic_cast<Player*>(&element))
	{
		for (auto it = this->entities.begin(); it != this->entities.end();)
		{
			Entity* entity = *it;
		}
	}

	const bool OUT_OF_BOUNDS = element.x > this->render_width || element.x < 0 || element.y > this->render_height || element.y < 0;
	if (OUT_OF_BOUNDS && element.type == Element::Spawnable::Projectile)
	{
		marked = true;
		goto DECIDE_FATE;
	}

DECIDE_FATE:
	if (marked) {
		this->to_delete.push_back(&element);
		return false;
	}
	else
	{
		return true;
	}
};

void Engine::process_spawns()
{
	for (auto& [to_spawn, owner] : this->to_spawn)
	{
		const int index = spawn(to_spawn, owner);

		Entity* entity = dynamic_cast<Entity*>(owner.element);
		if (entity->spawned_inheritance)
		{
			sf::FloatRect entity_bounds{};
			if (entity->shape != nullptr)
			{
				entity_bounds = entity->shape->getLocalBounds();
			}
			else if (entity->sprite != nullptr)
			{
				entity_bounds = entity->sprite->getLocalBounds();
			}

			const float y_offset = entity_bounds.height / 2.0f;

			sf::FloatRect spawned_entity_bounds{};
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

	// Clear the spawn array to prevent duplicates
	this->to_spawn = {};
};

void Engine::process_despawns()
{
	// Deleting them here to prevent the same thing as well
	for (Element* entity : this->to_delete)
	{
		this->despawn(*entity);
	}

	// Clear the array to prevent overlap
	this->to_delete = {};

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
	for (Entity* entity : this->entities)
	{
		entity->set_texture(*textures[(int)entity->type]);
	}
};

void Engine::display_all(sf::RenderWindow& window, sf::RenderTexture& buffer, sf::Shader& shader)
{
	// Draw entities onto the render texture
	this->draw_objects(window, buffer);
	this->draw_entities(window, buffer);

	// Update buffer content after all drawing operations are done
	if (OLD_SCHOOL)
	{
		buffer.display(); // Update the render texture

		// Draw the render texture with the shader applied
		sf::Sprite sprite;
		sprite.setTexture(buffer.getTexture());
		window.draw(sprite, &shader);
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
				for (Entity* entity : this->entities)
				{
					buffer.draw(*entity->shape);
				}
			}
			else
			{
				for (Entity* entity : this->entities)
				{
					window.draw(*entity->shape);
				}
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
			if (OLD_SCHOOL)
			{
				for (Element* object : this->objects)
				{
					buffer.draw(*object->shape);
				}
			}
			else
			{
				for (Element* object : this->objects)
				{
					window.draw(*object->shape);
				}
			}
			break;

		case RenderMode::TEXTURES:
			for (Element* object : this->objects)
			{
				window.draw(*object->sprite);
			}
			break;
	}
};

// Start up
int Engine::initialize(Window& window)
{
	srand((unsigned int)time(nullptr));
	printf("Random number generation initiated.");

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
	this->spawn(Element::Spawnable::Player, Element::Owner{ Element::Spawner::Engine, nullptr });

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
