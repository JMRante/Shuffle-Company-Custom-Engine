/*

    SHUFFLE COMPANY SOURCE CODE

    sc_entity.h
    ------------------------------------



*/

#ifndef SC_ENTITY
#define SC_ENTITY

#include <typeinfo>
#include <algorithm>
#include <vector>

#include "sc_log.h"
#include "sc_component.h"
#include "sc_nature.h"
//#include "sc_ui.h"
#include "sc_utility.h"

namespace sc
{
	template <class T>
	class ComponentPool
	{
	public:
		std::vector<T> pool;

		T* add(ID entityId, T component)
		{
			if (get(entityId) != NULL)
			{
				LOG_E << "Cannot add " << typeid(T).name() << ", entity " << entityId.get() << " already has one";
				return NULL;
			}

			component.entityId = entityId;
			pool.push_back(component);

			return &pool[pool.size() - 1];
		}

		bool remove(ID entityId)
		{
			for (auto ci = pool.begin(); ci != pool.end(); ci++)
			{
				if (ci->entityId.is(entityId))
				{
					pool.erase(ci);
					return true;
				}
			}

			LOG_E << "Cannot remove " << typeid(T).name() << ", entity " << entityId.get() << " doesn't have one";
			return false;
		}

		T* get(ID entityId)
		{
			for (auto ci = pool.begin(); ci != pool.end(); ci++)
			{
				if (ci->entityId.is(entityId))
				{
					return &(*ci);
				}
			}

			return NULL;
		}

		typename std::vector<T>::iterator begin()
		{
			return pool.begin();
		}

		typename std::vector<T>::iterator end()
		{
			return pool.end();
		}

		void copy(ComponentPool<T> otherPool)
		{
			pool = otherPool.pool;
		}
	};

	class EntityManager
	{
	private:
		std::vector<ID> entities;

	public:
		//Components
		ComponentPool<Transform> transformPool;
		ComponentPool<Camera> cameraPool;
		ComponentPool<DrawModel> drawModelPool;

		//Natures
		ComponentPool<DebugCamera> debugCameraPool;

		EntityManager();
		void copy(EntityManager* otherEM);
		bool addEntity(ID id);
		bool entityExists(ID id);
		bool removeEntity(ID id);
	};
}

#endif