#pragma once

#include <list>
#include "Entity.h"

class EntityManager {
public:
	EntityManager() = default;

	~EntityManager() = default;

public:
	static std::list<std::shared_ptr<Entity>> managerEntities;
	static std::list<std::shared_ptr<Entity>> GetCoins();
	static void RemoveCoin(std::shared_ptr<Entity> coin);
	static std::list<std::shared_ptr<Entity>> GetFloors();
	static std::list<std::shared_ptr<Entity>> GetLadders();
};

