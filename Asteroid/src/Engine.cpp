#define STRICT

#include <string>

#include "Window.h"
#include "Entity.h"
#include "Engine.h"


void Engine::start_game(Window* window)
{
	Entity* player = new Entity(Entity::Type::Player);
	player->velocity = -0.01f;
	this->add_entity(player);
};


void Engine::draw_entities(sf::RenderWindow* window)
{
	for (auto entity : this->entities) {
		window->draw(entity->sprite);
	}
};

void Engine::calculate_moves(Window* window)
{
	double* p_radians = new double((3.1415926536 / 180) * this->entities[0]->saved_rotation);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->entities[0]->projectile_cooldown == 0
	//&& !(this->entities[0]->x + this->entities[0]->velocity * -sin(*p_radians)) < 0
	//&& !(this->entities[0]->x + this->entities[0]->velocity * -sin(*p_radians)) > window->m_width
	//&& !(this->entities[0]->y + this->entities[0]->velocity * cos(*p_radians)) < 0
	//&& !(this->entities[0]->y + this->entities[0]->velocity * cos(*p_radians)) > window->m_height) {
		) {

		this->entities[0]->projectile_cooldown = 45;

		this->key_pressed++;
		Entity* projectile = new Entity(Entity::Type::Projectile);
		projectile->owner = "player";

		projectile->x = this->entities[0]->x + this->entities[0]->velocity * -sin(*p_radians) * 3;
		projectile->y = this->entities[0]->y + this->entities[0]->velocity * cos(*p_radians) * 3;

		projectile->rotation = this->entities[0]->rotation;

		projectile->velocity = this->entities[0]->velocity < 0 ? 
			this->entities[0]->velocity - 8 : this->entities[0]->velocity + 8;

		this->add_entity(projectile);
	}

	delete p_radians;

	std::vector<int> marked_for_deletion_indexes{};

	int i = 0;

	for (auto entity : this->entities) {
		switch (entity->type) {
			case Entity::Type::Player: {
				if (entity->v_dx < 0) {
					entity->v_dx += 0.005f;
				}

				if (entity->v_dx > 0) {
					entity->v_dx -= 0.005f;
				}

				if (entity->v_dy < 0) {
					entity->v_dy += 0.005f;
				}

				if (entity->v_dy > 0) {
					entity->v_dy -= 0.005f;
				}

				if (entity->rotation_change < 0) {
					entity->rotation_change += 0.07f;
				}

				if (entity->rotation_change > 0) {
					entity->rotation_change -= 0.07f;
				}

				double radians = (3.1415926536 / 180) * entity->saved_rotation;

				if (last_pressed_forward_flag) {
					entity->v_dx = entity->velocity * -sin(radians);
					entity->v_dy = entity->velocity * cos(radians);
				}

				entity->x += entity->v_dx;
				entity->y += entity->v_dy;

				entity->rotation += entity->rotation_change;

				// Debug information
				if (this->debug) {
					std::printf(("X: " + std::to_string(entity->x) +
						" Y: " + std::to_string(entity->y) +
						" vDx: " + std::to_string(entity->v_dx) +
						" vDy: " + std::to_string(entity->v_dy) +
						" Vel: " + std::to_string(entity->velocity) +
						" R: " + std::to_string(entity->rotation_change) +
						" Rc: " + std::to_string(entity->rotation_change) +
						" Key: " + std::to_string(this->key_pressed) +
						" Cd: " + std::to_string(entity->projectile_cooldown) +
						'\n').c_str());
				}

				if (abs(entity->velocity) > 5) {
					entity->velocity = entity->velocity < 0 ? -5 : 5;
				}

				if (abs(entity->rotation_change) > 8) {
					entity->rotation_change = entity->rotation_change < 0 ? -8 : 8;
				}

				if (abs(entity->rotation_change) < 0.4f && !key_pressed > 0) {
					entity->rotation_change = 0;
				}

				if (entity->x <= 0) {
					entity->x = window->m_width - 1;
				}

				if (entity->x >= window->m_width) {
					entity->x = 1;
				}

				if (entity->y >= window->m_height) {
					entity->y = 1;
				}

				if (entity->y <= 0) {
					entity->y = window->m_height - 1;
				}

				if (entity->projectile_cooldown != 0) {
					entity->projectile_cooldown--;
				}

				break;
			}

			case Entity::Type::Projectile: {

				double radians = (3.1415926536 / 180) * entity->rotation;

				entity->v_dx = entity->velocity * -sin(radians);
				entity->v_dy = entity->velocity * cos(radians);

				entity->x += entity->v_dx;
				entity->y += entity->v_dy;

				if (entity->y < 0 || entity->x < 0 || entity->x > window->m_height || entity->x > window->m_width) {
					marked_for_deletion_indexes.push_back(i);
				}

				break;
			}
		}

		i++;
	}

	for (int id : marked_for_deletion_indexes) {
		if (this->debug) {
			std::printf(("Entity " + std::to_string(id) + " | " + std::to_string(this->entities[id]->id) + " deleted!" + "\n").c_str());
		}

		this->entities.erase(this->entities.begin() + id);
	}
};


void Engine::execute_moves()
{
	for (auto entity : this->entities) {
		entity->sprite.rotate(entity->rotation_change);
		entity->sprite.setPosition(entity->x, entity->y);
	}
};


void Engine::add_entity(Entity* entity)
{
	this->entity_ids++;
	entity->id = this->entity_ids;

	this->entities.push_back(entity);

	if (this->debug) {
		std::printf(("Added entity " + std::to_string(this->entity_ids) + "\n").c_str());
	}
};


std::string Engine::load_textures(Window* window)
{

	sf::Texture player_texture;
	sf::Texture projectile_texture;

	if (!player_texture.loadFromFile("./textures/as_vec_player.png"))
		return "Error loading PLAYER TEXTURE: as_vec_player.png in /textures/\n";

	if (!projectile_texture.loadFromFile("./textures/as_vec_projectile.png"))
		return "Error loading PROJECTILE TEXTURE: as_vec_projectile.png in /textures/\n";

	textures.push_back(player_texture);
	std::printf("[TEXTURE LOADED] - PLAYER\n");

	textures.push_back(projectile_texture);
	std::printf("[TEXTURE LOADED] - PROJECTILE\n");

	return "";
};

