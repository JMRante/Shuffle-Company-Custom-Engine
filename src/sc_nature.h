/*

    SHUFFLE COMPANY SOURCE CODE

    sc_nature.h
    ------------------------------------



*/

#ifndef SC_NATURE
#define SC_NATURE

#include <vector>
#include <string>

#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "glm/ext.hpp"

#include "sc_log.h"
#include "sc_input.h"
#include "sc_component.h"
#include "sc_main.h"
#include "sc_utility.h"

namespace sc
{
	class Nature : public Component
	{
	public:
		bool isActive;

		Nature();
		// virtual void create() = 0;
		virtual void update(State* readState, State* writeState) = 0;
		// virtual void destory() = 0;
		
		void onStateInsert();
		void onStateRemove();
	};

	class DebugCamera : public Nature
	{
	private:
		float moveSpeed;
		float mouseSpeed;
		float yaw;
		float pitch;

	public:
		DebugCamera(float moveSpeed, float mouseSpeed);
		void update(State* readState, State* writeState);
	};

	class EditorCamera : public Nature
	{
	private:
		float keyMoveSpeed;
		float mouseMoveSpeed;

	public:
		EditorCamera(float keyMoveSpeed, float mouseMoveSpeed);
		void update(State* readState, State* writeState);
	};

	enum CursorState {point, hover, click, drag};
	class Cursor : public Nature
	{
	private:
		Sprite* pointSprite;
		Sprite* hoverSprite;
		Sprite* clickSprite;
		Sprite* dragSprite;

	public:
		CursorState cursorState;

		Cursor();
		void update(State* readState, State* writeState);
	};

	class FramerateCounter : public Nature
	{
	private:
		float framerateHistory[60] = {};
		int framerateHistoryCount;

	public:
		float framerateAverage;

		FramerateCounter();
		void update(State* readState, State* writeState);
	};
}

#endif