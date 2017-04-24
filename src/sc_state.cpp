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

			return true;
		}

		LOG_E << "Cannot add existing entity " << id.get();
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
			removeAllComponents(id);
			componentMap.erase(it);

			return true;
		}

		LOG_E << "Cannot remove non-existing entity " << id.get();
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

	void State::removeAllComponents(ID entityId)
	{
		std::vector<Component*>* coms = &componentMap[entityId];

		if (coms != NULL)
		{
			for (auto ci = coms->begin(); ci != coms->end(); ci++)
			{
				(*ci)->onStateRemove();
				delete *ci;
				coms->erase(ci);
			}	
		}		
	}
}