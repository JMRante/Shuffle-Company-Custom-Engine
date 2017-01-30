/*

    SHUFFLE COMPANY SOURCE CODE

    sc_gameElement.h
    ------------------------------------



*/

#ifndef SC_GAMEELEMENT
#define SC_GAMEELEMENT

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "sc_log.h"
#include "sc_assets.h"

namespace sc
{
	class GameElement
	{
	public:
		sc::Model *model;

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		GameElement(std::string modelId);
		GameElement(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca, std::string modelId);
		void update();
	};
}

#endif