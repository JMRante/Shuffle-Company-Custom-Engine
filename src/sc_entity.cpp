/*

    SHUFFLE COMPANY SOURCE CODE

    sc_entity.cpp
    ------------------------------------



*/

#include "sc_entity.h"

namespace sc
{
	/*
		Entity Management
							*/
	void EntityManager::copy(EntityManager* otherEM)
	{
		entities = otherEM->entities;

		transformPool = otherEM->transformPool;
		cameraPool = otherEM->cameraPool;
		drawPool = otherEM->drawPool;
		behaviorPool = otherEM->behaviorPool;
	}

	bool EntityManager::addEntity(std::string id)
	{
		if (!entityExists(id))
		{
			entities.push_back(id);
			Transform defaultTransform;
			defaultTransform.setEntityId(id);
			transformPool.push_back(defaultTransform);

			return true;
		}

		LOG_E << "Cannot add existing entity " << id;
		return false;
	}

	bool EntityManager::entityExists(std::string id)
	{
		for (size_t i = 0; i < entities.size(); i++)
		{
			if (entities[i] == id)
			{
				return true;
			}
		}

		return false;
	}

	bool EntityManager::removeEntity(std::string id)
	{
		for (size_t i = 0; i < entities.size(); i++)
		{
			if (entities[i] == id)
			{
				removeCamera(id);
				removeDraw(id);
				removeBehavior(id);

				for (size_t i = 0; i < transformPool.size(); i++)
				{
					if (transformPool[i].getEntityId() == id)
					{
						transformPool.erase(transformPool.begin() + i);
					}
				}

				entities.erase(entities.begin() + i);
				return true;
			}
		}

		LOG_E << "Cannot remove non-existing entity " << id;
		return false;
	}


	/*
		Add Components
						*/
	bool EntityManager::addCamera(std::string id, sc::Camera camera)
	{
		if (getCamera(id) != NULL)
		{
			LOG_E << "Cannot add camera, entity " << id << " already has one";
			return false; 
		}
		
		camera.setEntityId(id);
		cameraPool.push_back(camera);
		return true;
	}

	bool EntityManager::addDraw(std::string id, sc::Draw draw)
	{
		if (getDraw(id) != NULL)
		{
			LOG_E << "Cannot add draw, entity " << id << " already has one";
			return false; 
		}
		
		draw.setEntityId(id);
		drawPool.push_back(draw);
		return true;
	}

	bool EntityManager::addBehavior(std::string id, sc::Behavior* behavior)
	{
		if (getBehavior(id) != NULL)
		{
			LOG_E << "Cannot add behavior, entity " << id << " already has one";
			return false; 
		}
		
		behavior->setEntityId(id);
		behaviorPool.push_back(behavior);
		return true;
	}


	/*
		Remove Components
							*/
	bool EntityManager::removeCamera(std::string id)
	{
		for (size_t i = 0; i < cameraPool.size(); i++)
		{
			if (cameraPool[i].getEntityId() == id)
			{
				cameraPool.erase(cameraPool.begin() + i);
				return true;			
			}
		}

		LOG_E << "Cannot remove camera, entity " << id << " doesn't have one";
		return false;
	}

	bool EntityManager::removeDraw(std::string id)
	{
		for (size_t i = 0; i < drawPool.size(); i++)
		{
			if (drawPool[i].getEntityId() == id)
			{
				drawPool.erase(drawPool.begin() + i);
				return true;			
			}
		}		

		LOG_E << "Cannot remove draw, entity " << id << " doesn't have one";
		return false; 
	}

	bool EntityManager::removeBehavior(std::string id)
	{
		for (size_t i = 0; i < behaviorPool.size(); i++)
		{
			if (behaviorPool[i]->getEntityId() == id)
			{
				behaviorPool.erase(behaviorPool.begin() + i);
				return true;			
			}
		}

		LOG_E << "Cannot remove behavior, entity " << id << " doesn't have one";
		return false; 
	}


	/*
		Get individual components
									*/
	sc::Transform* EntityManager::getTransform(std::string id)
	{
		for (size_t i = 0; i < transformPool.size(); i++)
		{
			if (transformPool[i].getEntityId() == id)
			{
				return &transformPool[i];				
			}
		}

		LOG_E << "Could not find transform for entity " << id;
		return NULL;
	}

	sc::Camera* EntityManager::getCamera(std::string id)
	{
		for (size_t i = 0; i < cameraPool.size(); i++)
		{
			if (cameraPool[i].getEntityId() == id)
			{
				return &cameraPool[i];				
			}
		}

		LOG_E << "Could not find camera for entity " << id;
		return NULL;
	}

	sc::Draw* EntityManager::getDraw(std::string id)
	{
		for (size_t i = 0; i < drawPool.size(); i++)
		{
			if (drawPool[i].getEntityId() == id)
			{
				return &drawPool[i];				
			}
		}

		LOG_E << "Could not find draw for entity " << id;
		return NULL;
	}

	sc::Behavior* EntityManager::getBehavior(std::string id)
	{
		for (size_t i = 0; i < behaviorPool.size(); i++)
		{
			if (behaviorPool[i]->getEntityId() == id)
			{
				return behaviorPool[i];				
			}
		}

		LOG_E << "Could not find behavior for entity " << id;
		return NULL;
	}


	/*
		Get component pool iterators
									*/
	std::vector<sc::Transform>::iterator EntityManager::getTransformPoolBegin()
	{
		return transformPool.begin();
	}

	std::vector<sc::Camera>::iterator EntityManager::getCameraPoolBegin()
	{
		return cameraPool.begin();
	}

	std::vector<sc::Draw>::iterator EntityManager::getDrawPoolBegin()
	{
		return drawPool.begin();
	}

	std::vector<sc::Behavior*>::iterator EntityManager::getBehaviorPoolBegin()
	{
		return behaviorPool.begin();
	}



	std::vector<sc::Transform>::iterator EntityManager::getTransformPoolEnd()
	{
		return transformPool.end();
	}

	std::vector<sc::Camera>::iterator EntityManager::getCameraPoolEnd()
	{
		return cameraPool.end();
	}

	std::vector<sc::Draw>::iterator EntityManager::getDrawPoolEnd()
	{
		return drawPool.end();
	}

	std::vector<sc::Behavior*>::iterator EntityManager::getBehaviorPoolEnd()
	{
		return behaviorPool.end();
	}
}