#include "pch.h"
#include "EntityManager.h"


std::list<std::shared_ptr<Entity>> EntityManager::managerEntities;

std::list<std::shared_ptr<Entity>> EntityManager::GetCoins() {
	std::list<std::shared_ptr<Entity>> coins;

	for (auto const& entity : EntityManager::managerEntities) {
		if (entity->entityType == EntityType::coin) {
			coins.push_back(entity);
		}
	}

	return coins;
}

void EntityManager::RemoveCoin(std::shared_ptr<Entity> coin) {
	EntityManager::managerEntities.remove(coin);
}

std::list<std::shared_ptr<Entity>> EntityManager::GetFloors() {
	std::list<std::shared_ptr<Entity>> floors;

	for (auto const& entity : EntityManager::managerEntities) {
		if (entity->entityType == EntityType::block) {
			floors.push_back(entity);
		}
	}

	return floors;
}

std::list<std::shared_ptr<Entity>> EntityManager::GetLadders() {
	std::list<std::shared_ptr<Entity>> ladders;

	for (auto const& entity : EntityManager::managerEntities) {
		if (entity->entityType == EntityType::ladder) {
			ladders.push_back(entity);
		}
	}

	return ladders;
}