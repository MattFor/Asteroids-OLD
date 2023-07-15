#define STRICT

#include "Entity.h"


void Entity::get_texture(sf::Texture texture, Window* window)
{
	switch (this->type) {
		case Entity::Type::Player: {
			sf::Sprite player_sprite(texture);

			player_sprite.setOrigin((sf::Vector2f)texture.getSize() / 2.0f);

			this->sprite = player_sprite;
			this->texture = texture;
			this->sprite.setTexture(this->texture);

			this->x = window->m_width / 2;
			this->y = window->m_height / 2;

			this->sprite.setPosition(this->x, this->y);
			return;
		}

		case Entity::Type::Projectile: {
			sf::Sprite projectile_sprite(texture);

  			projectile_sprite.setOrigin((sf::Vector2f)texture.getSize() / 2.0f);

			this->sprite = projectile_sprite;
			this->texture = texture;
			this->sprite.setTexture(this->texture);
			return;
		}
	}
};
