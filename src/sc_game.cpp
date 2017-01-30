/*

    SHUFFLE COMPANY SOURCE CODE

    sc_game.cpp
    ------------------------------------



*/

#include "sc_game.h"

namespace sc
{
	void Game::start()
	{
		currentState = new sc::World();
		nextState = new sc::World();

		sc::Vertex tempVert;
		std::vector<sc::Vertex> vecVert;

		tempVert.position = glm::vec3(0.5f, 0.5f, 0.0f);
		tempVert.normal = glm::vec3(0.0f, 0.0f, -1.0f);
		tempVert.textureCoord = glm::vec2(0.0f, 0.0f);
		LOG_D << "Vertex: Pos-" << glm::to_string(tempVert.position) << " Normal-" << glm::to_string(tempVert.normal) << " UV-" << glm::to_string(tempVert.textureCoord);
		vecVert.push_back(tempVert);

		tempVert.position = glm::vec3(0.5f, -0.5f, 0.0f);
		LOG_D << "Vertex: Pos-" << glm::to_string(tempVert.position) << " Normal-" << glm::to_string(tempVert.normal) << " UV-" << glm::to_string(tempVert.textureCoord);
		vecVert.push_back(tempVert);

		tempVert.position = glm::vec3(-0.5f, -0.5f, 0.0f);
		LOG_D << "Vertex: Pos-" << glm::to_string(tempVert.position) << " Normal-" << glm::to_string(tempVert.normal) << " UV-" << glm::to_string(tempVert.textureCoord);
		vecVert.push_back(tempVert);

		tempVert.position = glm::vec3(-0.5f, 0.5f, 0.0f);
		LOG_D << "Vertex: Pos-" << glm::to_string(tempVert.position) << " Normal-" << glm::to_string(tempVert.normal) << " UV-" << glm::to_string(tempVert.textureCoord);
		vecVert.push_back(tempVert);

		static const int ind[] = {0, 1, 3, 1, 2, 3};
		std::vector<int> vecInd(ind, ind + sizeof(ind) / sizeof(ind[0]));

		sc::assets.loadMesh("ME_SQUARE", &vecVert, &vecInd);
		sc::assets.loadShader("SH_PASS", "Resources/Shaders/sc_shader_testVertex.glsl", "Resources/Shaders/sc_shader_testFragment.glsl");
		sc::assets.loadMaterial("MA_TEST", "SH_PASS");
		sc::assets.loadModel("MO_TEST", "ME_SQUARE", "MA_TEST");

		currentState->elements.push_back(sc::GameElement("MO_TEST"));
	}

	bool Game::update()
	{
		//Handle events
		SDL_Event event;

		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				return true; //Do not continue to update (close game)
			}
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_UP:
						LOG_D << SDLK_UP;
						break;

					case SDLK_DOWN:
						LOG_D << SDLK_DOWN;
						break;

					case SDLK_LEFT:
						LOG_D << SDLK_LEFT;
						break;

					case SDLK_RIGHT:
						LOG_D << SDLK_RIGHT;
						break;

					case SDLK_RETURN:
						LOG_D << SDLK_RETURN;
						break;

					case SDLK_ESCAPE:
						LOG_D << SDLK_ESCAPE;
						break;

					case SDLK_BACKQUOTE:
						LOG_D << SDLK_BACKQUOTE;
						break;

					default:
						break;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				switch (event.button.button)
				{
					case SDL_BUTTON_LEFT:
						LOG_D << SDL_BUTTON_LEFT << " Down";
						break;
					case SDL_BUTTON_MIDDLE:
						LOG_D << SDL_BUTTON_MIDDLE << " Down";
						break;
					case SDL_BUTTON_RIGHT:
						LOG_D << SDL_BUTTON_RIGHT << " Down";
						break;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				switch (event.button.button)
				{
					case SDL_BUTTON_LEFT:
						LOG_D << SDL_BUTTON_LEFT << " Up";
						break;
					case SDL_BUTTON_MIDDLE:
						LOG_D << SDL_BUTTON_MIDDLE << " Up";
						break;
					case SDL_BUTTON_RIGHT:
						LOG_D << SDL_BUTTON_RIGHT << " Up";
						break;
				}				
			}
		}

		return false; //Continue to update (game not yet closed)
	}
}