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
#include "sc_stage.h"
#include "sc_id.h"

namespace sc
{
	class Nature : public Component
	{
	public:
		Nature();

		virtual void update() = 0;
		
		void onStateInsert();
		void onStateRemove();
	};

	enum CursorState {point, hover, click, drag};
	class Cursor : public Nature
	{
	private:
		Transform* tran;
		DrawSprite* drawSprite;

		Sprite* pointSprite;
		Sprite* hoverSprite;
		Sprite* clickSprite;
		Sprite* dragSprite;
		ID focus;

	public:
		CursorState cursorState;

		Cursor();
		void create();
		void update();
		ID getFocus();
	};

	class DebugCamera : public Nature
	{
	private:
		Transform* tran;
		Camera* camera;

		float moveSpeed;
		float mouseSpeed;
		float yaw;
		float pitch;

	public:
		DebugCamera(float moveSpeed, float mouseSpeed);
		void create();
		void update();
	};

	class EditorCamera : public Nature
	{
	private:
		Transform* tran;
		Camera* camera;
		Stage* stage;
		std::vector<Transform*> editSlotTransforms;

		float keyMoveSpeed;
		float mouseMoveSpeed;
		int cameraLayer;

	public:
		EditorCamera(float keyMoveSpeed, float mouseMoveSpeed);
		void create();
		void update();
		int getCameraLayer();
	};

	class EditorSlot : public Nature
	{
	private:
		DrawModel* drawModel;
		Stage* stage;
		EditorCamera* ec;
		EditorOperationManager* eom;

		int x;
		int z;

	public:
		EditorSlot(int x, int z);
		void create();
		void update();
	};

	class EditorControl : public Nature
	{
	private:
		EditorOperationManager* eom;
		Stage* stage;

	public:
		EditorControl();
		void create();
		void update();
	};

	class EditorTextField : public Nature
	{
	private:
		DrawRectangle* box;
		DrawText* text;
		Cursor* cursor;
		Transform* textCursorTran;
		DrawText* textCursor;

		Event* editEvent;
		Event* finishEvent;
		bool focused;
		size_t sizeLimit;

	public:
		EditorTextField(Event* editEvent, Event* finishEvent, size_t sizeLimit);
		void create();
		void update();
	};

	class FramerateCounter : public Nature
	{
	private:
		DrawText* drawText;

		float framerateHistory[60] = {};
		int framerateHistoryCount;

	public:
		float framerateAverage;

		FramerateCounter();
		void create();
		void update();
	};

	class Button : public Nature
	{
	private:
		DrawRectangle* fore;

		float highlightSpeed;
		float highlightPosition;
		glm::vec4 normalColor;
		glm::vec4 highlightColor;
		Event* event;

	public:
		Button(Event* event);
		~Button();
		void create();
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