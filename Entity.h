#pragma once

#include "Animation.h"
#include "AnimationSprite.h"

enum EntityType {
	player,
	block,
	ladder,
	coin,
	unknown
};

class Entity {
public:
	Entity(bool isAnimated, EntityType type) :
		isAnimated(isAnimated),
		m_type(type)
	{}
public:
	sf::Sprite m_sprite;
	sf::Vector2u m_size;
	sf::Vector2f m_position;
	EntityType m_type = EntityType::unknown;
	Animation* currentAnimation;
	AnimationSprite animatedSprite;
	bool isAnimated = false;
	bool isMoving = false;

	// Enemy only
	bool m_bLeftToRight = true;
	int m_times = 0;
};

