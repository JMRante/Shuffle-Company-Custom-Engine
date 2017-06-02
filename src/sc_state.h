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
		std::map<ID, std::vector<ID>> tagMap;

	public:
		std::vector<Nature*> naturePointers;
		std::vector<DrawModel*> modelPointers;
		std::vector<DrawModel*> transparentModelPointers;
		std::vector<DrawOrtho*> orthoPointers;

		std::vector<DrawModel*> mouseSelectModels;
		std::vector<DrawOrtho*> mouseSelectOrthos;

		State();

		bool addEntity(ID id);
		bool entityExists(ID id);
		bool removeEntity(ID id);
		void removeAllEntities(ID id);

		void addEntityTag(ID entityId, ID tagId);
		bool entityHasTag(ID entityId, ID tagId);
		void removeEntityTag(ID entityId, ID tagId);

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

		template <class T>
		std::vector<T*> addComponentToTagged(ID tagId, T* component)
		{
			std::vector<T*> components;

			for (auto ei = tagMap.begin(); ei != tagMap.end(); ei++)
			{
				if (entityHasTag(ei->first, tagId))
				{
					components.push_back(addComponent<T>(ei->first, component));
				}
			}

			return components;
		}

		template <class T>
		std::vector<T*> getComponentFromTagged(ID tagId)
		{
			std::vector<T*> components;

			for (auto ei = tagMap.begin(); ei != tagMap.end(); ei++)
			{
				if (entityHasTag(ei->first, tagId))
				{
					components.push_back(getComponent<T>(ei->first));
				}
			}

			return components;
		}

		template <class T>
		bool removeComponentFromTagged(ID tagId)
		{
			bool success = true;

			for (auto ei = tagMap.begin(); ei != tagMap.end(); ei++)
			{
				if (entityHasTag(ei->first, tagId))
				{
					if (!removeComponent<T>(ei->first))
					{
						success = false;
					}
				}
			}

			if (success)
			{
				return true;
			}
			else
			{
				LOG_E << "Cannot remove " << typeid(T).name() << " from all tagged entities";
				return false;
			}
		}

		void removeAllComponents(ID entityId);
		void removeAllComponentsFromTagged(ID tagId);
	};
}

#endif