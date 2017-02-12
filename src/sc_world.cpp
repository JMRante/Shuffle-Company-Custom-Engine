/*

    SHUFFLE COMPANY SOURCE CODE

    sc_world.cpp
    ------------------------------------



*/

#include "sc_world.h"
#include "sc_entity.h"

namespace sc
{
	void World::copy(World* otherWorld)
	{
		entityManager.copy(&otherWorld->entityManager);
	}
}