/*

    SHUFFLE COMPANY SOURCE CODE

    sc_gameElement.cpp
    ------------------------------------



*/

#include "sc_gameElement.h"

namespace sc
{
	GameElement::GameElement(std::string modelId)
	{
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
		model = assets.getModel(modelId);
	}

	GameElement::GameElement(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca, std::string modelId)
	{
		position = pos;
		rotation = rot;
		scale = sca;
		model = assets.getModel(modelId);
	}

	void GameElement::update()
	{

	}
}