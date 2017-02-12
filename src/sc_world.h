/*

    SHUFFLE COMPANY SOURCE CODE

    sc_world.h
    ------------------------------------



*/

#ifndef SC_WORLD
#define SC_WORLD

#include <vector>

#include "sc_log.h"
#include "sc_entity.h"
#include "sc_stage.h"

namespace sc
{
	class World
	{
	public:
		sc::EntityManager entityManager;
		sc::Stage stage;

		void copy(World* otherWorld);
	};
}

#endif