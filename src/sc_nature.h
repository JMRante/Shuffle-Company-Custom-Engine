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
#include "sc_events.h"
#include "sc_color.h"

namespace sc
{
	class Nature : public Component
	{
	public:
		Nature();
		//virtual void create() = 0;
		virtual void update() = 0;
		//virtual void destory() = 0;
		
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
		void update();
	};

	class EditorCamera : public Nature
	{
	private:
		float keyMoveSpeed;
		float mouseMoveSpeed;
		int cameraLayer;
		std::vector<Transform*> editSlotTransforms;

	public:
		EditorCamera(float keyMoveSpeed, float mouseMoveSpeed);
		void update();
		int getCameraLayer();
	};

	class EditorSlot : public Nature
	{
	private:
		int x;
		int z;

	public:
		EditorSlot(int x, int z);
		void update();
	};

	class EditorShortcuts : public Nature
	{
	public:
		EditorShortcuts();
		void update();
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
		void update();
	};

	class FramerateCounter : public Nature
	{
	private:
		float framerateHistory[60] = {};
		int framerateHistoryCount;

	public:
		float framerateAverage;

		FramerateCounter();
		void update();
	};

	class Button : public Nature
	{
	private:
		float highlightSpeed;
		float highlightPosition;
		glm::vec4 normalColor;
		glm::vec4 highlightColor;
		Event* event;

	public:
		Button(Event* event);
		~Button();
		void update();
	};

	class FileSelector : public Nature
	{

	};

	class FileSelectorItem : public Nature
	{
	private:
		std::string fileName;

	public:
		FileSelectorItem();
		void update();
	};
}

#endif