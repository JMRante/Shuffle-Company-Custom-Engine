/*

    SHUFFLE COMPANY SOURCE CODE

    sc_prefabs.h
    ------------------------------------



*/

#ifndef SC_PREFABS
#define SC_PREFABS

#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"

#include "sc_log.h"
#include "sc_utility.h"
#include "sc_state.h"
#include "sc_component.h"
#include "sc_game.h"

namespace sc
{
	ID createStage(std::string filepath);

	ID createDebugCamera(ID id, glm::vec3 position, glm::vec3 rotation, float near, float far, float moveSpeed, float turnSpeed);
	ID createEditorCamera(ID id, glm::vec3 position, float pitch, float near, float far, float keyMoveSpeed, float mouseMoveSpeed);
	ID createFramerateCounter(ID id, glm::vec2 position, glm::vec4 color, ID fontId);

	ID createUIRectangle(ID id, glm::vec2 position, glm::vec2 size, glm::vec2 pivot, glm::vec4 color);
	ID createUISprite(ID id, glm::vec2 position, glm::vec2 scale, glm::vec2 pivot, ID spriteId);
	ID createUIText(ID id, glm::vec2 position, std::string text, ID fontId, glm::vec4 color);

	ID createCursor();
}

#endif