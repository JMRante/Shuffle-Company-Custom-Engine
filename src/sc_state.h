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
		State* state;

		T* add(ID entityId, T component)
		{
			if (get(entityId) != NULL)
			{
				LOG_E << "Cannot add " << typeid(T).name() << ", entity " << entityId.get() << " already has one";
				return NULL;
			}

			component.entityId = entityId;
			component.state = state;
			pool.push_back(component);
			T* cPointer = &pool[pool.size() - 1];

			cPointer->onStateInsert();

			return cPointer;
		}

		bool remove(ID entityId)
		{
			for (auto ci = pool.begin(); ci != pool.end(); ci++)
			{
				if (ci->entityId.is(entityId))
				{
					ci->onStateRemove();

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
			state = otherPool.state;
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
		ComponentPool<DebugCamera> debugCameraPool;
		ComponentPool<EditorCamera> editorCameraPool;
		ComponentPool<Cursor> cursorPool;
		ComponentPool<FramerateCounter> framerateCounterPool;

		//Singletons
		Stage stage;

		//Pointer Lists
		std::vector<Nature*> naturePointers;
		std::vector<OrthoDraw*> orthoPointers;

		State();
		void copy(State* otherEM);
		bool addEntity(ID id);
		bool entityExists(ID id);
		bool removeEntity(ID id);
	};
}

#endif