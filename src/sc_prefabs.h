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
#include "sc_events.h"
#include "sc_config.h"
#include "sc_color.h"
#include "sc_id.h"

namespace sc
{
	class PrefabFactory
	{
	public:
		State* targetState;

		PrefabFactory(State* targetState);

		ID createStage(std::string filepath);

		ID createDebugCamera(ID id, glm::vec3 position, glm::vec3 rotation, float near, float far, float moveSpeed, float turnSpeed);
		ID createEditorCamera(ID id, glm::vec3 position, float pitch, float near, float far, float keyMoveSpeed, float mouseMoveSpeed);
		ID createFramerateCounter(ID id, glm::vec2 position, glm::vec4 color, ID fontId);

		ID createUIRectangle(ID id, glm::vec2 position, glm::vec2 size, glm::vec2 pivot, glm::vec4 color);
		ID createUISprite(ID id, glm::vec2 position, glm::vec2 scale, glm::vec2 pivot, ID spriteId);
		ID createUIText(ID id, glm::vec2 position, std::string text, glm::vec4 color, ID fontId, TextHAlign ha, TextVAlign va, TextHAlign just);

		ID createEditor(ID id);
		ID createEditorSlot(ID id, glm::vec3 position, int x, int z);

		ID createEditorPanel(ID id, glm::vec2 position, int layer, glm::vec2 size);
		ID createEditorLabel(ID id, glm::vec2 position, ID fontId, std::string text);
		ID createEditorSpriteButton(ID id, glm::vec2 position, int layer, glm::vec2 size, ID spriteId, Event* event);
		ID createEditorTextButton(ID id, glm::vec2 position, int layer, ID fontId, std::string text, Event* event);
		ID createEditorTextField(ID id, glm::vec2 position, int layer, size_t length, ID fontId, std::string startText, Event* editEvent, Event* finishEvent);
		ID createEditorFileSelector(ID id, std::string startPath, Event* selectEvent);

		ID createCursor();
	};
}

#endif