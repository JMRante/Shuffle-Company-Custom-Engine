/*

    SHUFFLE COMPANY SOURCE CODE

    sc_entity.cpp
    ------------------------------------



*/

#include "sc_entity.h"

namespace sc
{
	EntityManager::EntityManager() {}

	void EntityManager::copy(EntityManager* otherEM)
	{
		entities = otherEM->entities;

		transformPool.copy(otherEM->transformPool);
		cameraPool.copy(otherEM->cameraPool);
		drawModelPool.copy(otherEM->drawModelPool);
		drawRectanglePool.copy(otherEM->drawRectanglePool);

		debugCameraPool.copy(otherEM->debugCameraPool);
	}

	bool EntityManager::addEntity(ID id)
	{
		if (!entityExists(id))
		{
			entities.push_back(id);
			return true;
		}

		LOG_E << "Cannot add existing entity " << id.get();
		return false;
	}

	bool EntityManager::entityExists(ID id)
	{
		for (auto ei = entities.begin(); ei != entities.end(); ei++)
		{
			if (ei->is(id))
			{
				return true;
			}
		}

		return false;
	}

	bool EntityManager::removeEntity(ID id)
	{
		for (auto ei = entities.begin(); ei != entities.end(); ei++)
		{
			if (ei->is(id))
			{
				transformPool.remove(id);
				cameraPool.remove(id);
				drawModelPool.remove(id);
				drawRectanglePool.remove(id);

				debugCameraPool.remove(id);

				entities.erase(ei);
				return true;
			}
		}

		LOG_E << "Cannot remove non-existing entity " << id.get();
		return false;
	}
}