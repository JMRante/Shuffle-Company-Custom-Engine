/*

    SHUFFLE COMPANY SOURCE CODE

    sc_state.h
    ------------------------------------



*/

#ifndef SC_STATE
#define SC_STATE

#include <typeinfo>
#include <algorithm>
#include <vector>

#include "sc_log.h"
#include "sc_component.h"
#include "sc_nature.h"
#include "sc_utility.h"
#include "sc_stage.h"

namespace sc
{
	template <class T>
	class ComponentPool
	{
	private:
		std::vector<T> pool;

	public:
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

	class NaturePoolBase
	{
	private:
		static std::vector<Nature*> pointers;

	public:
		static std::vector<Nature*>::iterator begin()
		{
			return pointers.begin();
		}

		static std::vector<Nature*>::iterator end()
		{
			return pointers.end();
		}		
	};

	template <class T>
	class NaturePool : public NaturePoolBase
	{
	private:

		std::vector<T> pool;

	public:
		T* add(ID entityId, T nature)
		{
			if (get(entityId) != NULL)
			{
				LOG_E << "Cannot add " << typeid(T).name() << ", entity " << entityId.get() << " already has one";
				return NULL;
			}

			nature.entityId = entityId;
			pool.push_back(nature);

			T* naturePointer = &pool[pool.size() - 1];
			pointers.push_back(naturePointer);

			return naturePointer;
		}

		bool remove(ID entityId)
		{
			int poolDepth = 0;

			for (auto ni = pool.begin(); ni != pool.end(); ni++)
			{
				if (ni->entityId.is(entityId))
				{
					pool.erase(ni);
					pointers.erase(pointers.begin() + poolDepth);
					return true;
				}

				poolDepth++;
			}

			LOG_E << "Cannot remove " << typeid(T).name() << ", entity " << entityId.get() << " doesn't have one";
			return false;
		}

		T* get(ID entityId)
		{
			for (auto ni = pool.begin(); ni != pool.end(); ni++)
			{
				if (ni->entityId.is(entityId))
				{
					return &(*ni);
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

		void copy(NaturePool<T> otherPool)
		{
			pool = otherPool.pool;
		}
	};

	class State
	{
	private:
		std::vector<ID> entities;

	public:
		//Components
		ComponentPool<Transform> transformPool;
		ComponentPool<Camera> cameraPool;
		ComponentPool<DrawModel> drawModelPool;
		ComponentPool<DrawRectangle> drawRectanglePool;
		ComponentPool<DrawSprite> drawSpritePool;
		ComponentPool<DrawText> drawTextPool;

		//Natures
		NaturePool<DebugCamera> debugCameraPool;
		NaturePool<FramerateCounter> framerateCounterPool;

		//Singletons
		Stage stage;

		State();
		void copy(State* otherEM);
		bool addEntity(ID id);
		bool entityExists(ID id);
		bool removeEntity(ID id);
	};
}

#endif