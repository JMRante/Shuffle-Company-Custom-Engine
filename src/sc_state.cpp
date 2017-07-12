/*

    SHUFFLE COMPANY SOURCE CODE

    sc_state.cpp
    ------------------------------------



*/

#include "sc_state.h"

namespace sc
{
	State::State() {}

	bool State::addEntity(ID id)
	{
		if (!entityExists(id))
		{
			std::vector<Component*> components;
			componentMap.insert(std::pair<ID, std::vector<Component*>>(id, components));
			addComponent<Transform>(id, new Transform());

			return true;
		}

		LOG_E << "Cannot add existing entity";
		return false;
	}

	bool State::entityExists(ID id)
	{
		if (componentMap.find(id) != componentMap.end())
		{
			return true;
		}

		return false;
	}

	bool State::removeEntity(ID id)
	{
		auto it = componentMap.find(id);

		if (it != componentMap.end())
		{
			Transform* tran = getComponent<Transform>(id);
			
			if (tran->children.size() != 0)
			{
				for (auto et = tran->children.begin(); et != tran->children.end(); et++)
				{
					removeEntity((*et)->entityId);
				}
			}

			removeAllComponents(id);
			componentMap.erase(it);

			return true;
		}

		LOG_E << "Cannot remove non-existing entity";
		return false;
	}

	void State::removeAllEntities(ID id)
	{
		for (auto et = componentMap.begin(); et != componentMap.end(); et++)
		{
			removeAllComponents(et->first);
			componentMap.erase(et);
		}
	}

	void State::addEntityTag(ID entityId, ID tagId)
	{
		if (tagMap.find(entityId) == tagMap.end())
		{
			std::vector<ID> tags;
			tags.push_back(tagId);
			tagMap.insert(std::pair<ID, std::vector<ID>>(entityId, tags));
		}
		else
		{
			tagMap[entityId].push_back(tagId);
		}
	}

	bool State::entityHasTag(ID entityId, ID tagId)
	{
		if (tagMap.find(entityId) != tagMap.end())
		{
			std::vector<ID>* entityTags = &tagMap[entityId];
			
			if (find(entityTags->begin(), entityTags->end(), tagId) != entityTags->end())
			{
				return true;
			}
		}

		return false;	
	}

	void State::removeEntityTag(ID entityId, ID tagId)
	{
		if (tagMap.find(entityId) != tagMap.end())
		{
			std::vector<ID>* entityTags = &tagMap[entityId];

			if (entityTags != NULL)
			{
				auto it = find(entityTags->begin(), entityTags->end(), tagId);

				if (it != entityTags->end())
				{
					entityTags->erase(it);
					return;
				}
			}
		}
	}

	void State::removeAllComponents(ID entityId)
	{
		std::vector<Component*>* coms = &componentMap[entityId];

		if (coms != NULL)
		{
			for (auto ci = coms->begin(); ci != coms->end(); ci++)
			{
				if (typeid(**ci) != typeid(Transform))
				{
					(*ci)->onStateRemove();
					delete *ci;
					coms->erase(ci);
				}
			}
		}
	}

	void State::removeAllComponentsFromTagged(ID tagId)
	{
		for (auto ei = tagMap.begin(); ei != tagMap.end(); ei++)
		{
			if (entityHasTag(ei->first, tagId))
			{
				removeAllComponents(ei->first);
			}
		}
	}
}