/*

    SHUFFLE COMPANY SOURCE CODE

    sc_gameElement.h
    ------------------------------------



*/

#ifndef SC_GAMEELEMENT
#define SC_GAMEELEMENT

#include "sc_log.h"
#include <glm/glm.hpp>

namespace sc
{
	class GameElement
	{
	public:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};
}

#endif