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
		entityType(type)
	{}
public:
	sf::Sprite entitySprite;
	sf::Vector2u entitySize;
	sf::Vector2f entityPosition;
	EntityType entityType = EntityType::unknown;
	Animation* currentAnimation;
	AnimationSprite animatedSprite;
	bool isAnimated = false;
	bool isMoving = false;
};

