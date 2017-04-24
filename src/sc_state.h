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
#include <map>
#include <utility>

#include "sc_log.h"
#include "sc_component.h"
#include "sc_nature.h"
#include "sc_utility.h"
#include "sc_stage.h"

namespace sc
{
	class State
	{
	private:
		std::map<ID, std::vector<Component*>> componentMap;

	public:
		Stage stage;

		std::vector<Nature*> naturePointers;
		std::vector<DrawModel*> modelPointers;
		std::vector<DrawOrtho*> orthoPointers;

		State();

		bool addEntity(ID id);
		bool entityExists(ID id);
		bool removeEntity(ID id);
		void removeAllEntities(ID id);

		template <class T>
		T* addComponent(ID entityId, T* component)
		{
			if (getComponent<T>(entityId) != NULL)
			{
				LOG_E << "Cannot add " << typeid(T).name() << ", entity " << entityId.get() << " already has one";
				return NULL;
			}

			component->entityId = entityId;
			component->state = this;

			component->onStateInsert();
			
			componentMap[entityId].push_back((Component*) component);

			return component;
		}

		template <class T>
		T* getComponent(ID entityId)
		{
			std::vector<Component*>* coms = &componentMap[entityId];

			if (coms != NULL)
			{
				for (auto ci = coms->begin(); ci != coms->end(); ci++)
				{
					if (typeid(T) == typeid(**ci))
					{
						return static_cast<T*>(*ci);
					}
				}
			}

			return NULL;
		}

		template <class T>
		bool removeComponent(ID entityId)
		{
			std::vector<Component*>* coms = &componentMap[entityId];

			if (coms != NULL)
			{
				for (auto ci = coms->begin(); ci != coms->end(); ci++)
				{
					if (typeid(T) == typeid(**ci))
					{
						(*ci)->onStateRemove();
						delete *ci;
						coms->erase(ci);

						return true;
					}
				}	
			}

			LOG_E << "Cannot remove " << typeid(T).name() << ", entity " << entityId.get() << " doesn't have one";
			return false;
		}

		void removeAllComponents(ID entityId);
	};
}

#endif