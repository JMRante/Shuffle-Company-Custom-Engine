/*

    SHUFFLE COMPANY SOURCE CODE

    sc_prefabs.cpp
    ------------------------------------



*/

#include "sc_prefabs.h"

namespace sc
{
	PrefabFactory::PrefabFactory(State* targetState)
	{
		this->targetState = targetState;
	}

	ID PrefabFactory::createStage(std::string filepath)
	{
		ID id = ID("E_STAGE");

		targetState->addEntity(id);
		targetState->addComponent<Stage>(id, new Stage());
		targetState->addComponent<Transform>(id, new Transform());
		targetState->addComponent<DrawModel>(id, new DrawModel(ID("MO_STAGE")));

		return id;
	}

	ID PrefabFactory::createDebugCamera(ID id, glm::vec3 position, glm::vec3 rotation, float near, float far, float moveSpeed, float turnSpeed)
	{
		targetState->addEntity(id);

		Transform* tran = targetState->addComponent<Transform>(id, new Transform());
		tran->setPos(position);
		tran->setRot(rotation);

		Camera* camera = targetState->addComponent<Camera>(id, new Camera(near, far));
		camera->calculateViewMatrix();

		targetState->addComponent<DebugCamera>(id, new DebugCamera(moveSpeed, turnSpeed));

		return id;
	}

	ID PrefabFactory::createEditorCamera(ID id, glm::vec3 position, float pitch, float near, float far, float keyMoveSpeed, float mouseMoveSpeed)
	{
		targetState->addEntity(id);

		Transform* tran = targetState->addComponent<Transform>(id,new Transform());
		tran->setPos(position);
		tran->setRot(glm::vec3(glm::radians(pitch), glm::radians(180.0f), 0.0f));

		Camera* camera = targetState->addComponent<Camera>(id, new Camera(near, far));
		camera->calculateViewMatrix();

		targetState->addComponent<EditorCamera>(id, new EditorCamera(keyMoveSpeed, mouseMoveSpeed));

		return id;
	}

	ID PrefabFactory::createFramerateCounter(ID id, glm::vec2 position, glm::vec4 color, ID fontId)
	{
		targetState->addEntity(id);
		targetState->addComponent<Transform>(id, new Transform());
		DrawText* dt = targetState->addComponent<DrawText>(id, new DrawText());
		dt->initialize(position.x, position.y, "", color, fontId);
		targetState->addComponent<FramerateCounter>(id, new FramerateCounter());

		return id;
	}

	ID PrefabFactory::createUIRectangle(ID id, glm::vec2 position, glm::vec2 size, glm::vec2 pivot, glm::vec4 color)
	{
		targetState->addEntity(id);
		targetState->addComponent<Transform>(id, new Transform());
		DrawRectangle* dr = targetState->addComponent<DrawRectangle>(id, new DrawRectangle());
		dr->initialize(position.x, position.y, size.x, size.y, pivot.x, pivot.y, color);

		return id;
	}

	ID PrefabFactory::createUISprite(ID id, glm::vec2 position, glm::vec2 scale, glm::vec2 pivot, ID spriteId)
	{
		targetState->addEntity(id);
		targetState->addComponent<Transform>(id, new Transform());
		DrawSprite* ds = targetState->addComponent<DrawSprite>(id, new DrawSprite());
		ds->initialize(position.x, position.y, scale.x, scale.y, pivot.x, pivot.y, spriteId);

		return id;
	}

	ID PrefabFactory::createUIText(ID id, glm::vec2 position, std::string text, glm::vec4 color, ID fontId, TextHAlign ha, TextVAlign va, TextHAlign just)
	{
		targetState->addEntity(id);
		targetState->addComponent<Transform>(id, new Transform());
		DrawText* dt = targetState->addComponent<DrawText>(id, new DrawText());
		dt->initialize(position.x, position.y, text, color, fontId);
		dt->hAlignment = ha;
		dt->vAlignment = va;
		dt->justification = just;

		return id;
	}
	
	ID PrefabFactory::createEditor(ID id)
	{
		targetState->addEntity(id);
		targetState->addComponent<EditorOperationManager>(id, new EditorOperationManager(64));
		targetState->addComponent<EditorControl>(id, new EditorControl());

		return id;
	}

	ID PrefabFactory::createEditorSlot(ID id, glm::vec3 position, int x, int z)
	{
		targetState->addEntity(id);
		targetState->addEntityTag(id, ID("T_EDITSLOT"));

		Transform* tran = targetState->addComponent<Transform>(id, new Transform());
		tran->setPos(position);

		DrawModel* dm = targetState->addComponent<DrawModel>(id, new DrawModel(ID("MO_EDITSLOTA")));
		dm->addToMouseSelectable();

		targetState->addComponent<EditorSlot>(id, new EditorSlot(x, z));

		return id;
	}

	ID PrefabFactory::createEditorPanel(ID id, glm::vec2 position, int layer, glm::vec2 size)
	{
		targetState->addEntity(id);
		targetState->addComponent<Transform>(id, new Transform());
		DrawRectangle* dr = targetState->addComponent<DrawRectangle>(id, new DrawRectangle());
		dr->initialize(position.x, position.y, size.x, size.y, 0.0f, 0.0f, COL_EDITOR_UI_FOREGROUND);
		dr->setLayer(layer);

		return id;
	}

	ID PrefabFactory::createEditorLabel(ID id, glm::vec2 position, ID fontId, std::string text)
	{
		targetState->addEntity(id);
		targetState->addComponent<Transform>(id, new Transform());
		DrawText* dt = targetState->addComponent<DrawText>(id, new DrawText());
		dt->initialize(position.x, position.y, text, COL_WHITE, fontId);

		return id;
	}

	ID PrefabFactory::createEditorSpriteButton(ID id, glm::vec2 position, int layer, glm::vec2 size, ID spriteId, Event* event)
	{
		targetState->addEntity(id);
		targetState->addEntityTag(id, ID("T_BUTTON"));
		targetState->addEntityTag(id, ID("T_MAINEDIT"));

		Transform* tran1 = targetState->addComponent<Transform>(id, new Transform());
		DrawRectangle* dr = targetState->addComponent<DrawRectangle>(id, new DrawRectangle());
		dr->initialize(position.x, position.y, size.x, size.y, 0.0f, 0.0f, COL_EDTIOR_UI_BUTTONEDGE);
		dr->setLayer(layer);
		dr->addToMouseSelectable();
		targetState->addComponent<Button>(id, new Button(event));
		
		ID foreId = ID(id.getStr() + "FORE");
		targetState->addEntity(foreId);
		Transform* tran2 = targetState->addComponent<Transform>(foreId, new Transform());
		tran2->setParent(tran1);
		DrawRectangle* dr2 = targetState->addComponent<DrawRectangle>(foreId, new DrawRectangle());
		dr2->initialize(2.0f, 2.0f, size.x - 4.0f, size.y - 4.0f, 0.0f, 0.0f, COL_EDITOR_UI_BUTTON);
		dr2->setLayer(layer + 1);

		ID sprId = ID(id.getStr() + "SP");
		targetState->addEntity(sprId);
		Transform* tran3 = targetState->addComponent<Transform>(sprId, new Transform());
		tran3->setParent(tran2);
		DrawSprite* ds = targetState->addComponent<DrawSprite>(sprId, new DrawSprite());
		ds->initialize(-2.0f, -2.0f, 1.0f, 1.0f, 0.0f, 0.0f, spriteId);
		ds->setLayer(layer + 2);

		return id;
	}

	ID PrefabFactory::createEditorTextButton(ID id, glm::vec2 position, int layer, ID fontId, std::string text, Event* event)
	{
		targetState->addEntity(id);
		targetState->addEntityTag(id, ID("T_BUTTON"));

		ID textId = ID(id.getStr() + "TX");
		targetState->addEntity(textId);
		Transform* tran3 = targetState->addComponent<Transform>(textId, new Transform());
		DrawText* dt = targetState->addComponent<DrawText>(textId, new DrawText());
		dt->initialize(4.0f, 4.0f, text, COL_WHITE, fontId);
		dt->setLayer(layer + 2);
		float textWidth = dt->getWidth();
		float textHeight = dt->getHeight();

		Transform* tran1 = targetState->addComponent<Transform>(id, new Transform());
		DrawRectangle* dr = targetState->addComponent<DrawRectangle>(id, new DrawRectangle());
		dr->initialize(position.x, position.y, textWidth + 12.0f, textHeight + 4.0f, 0.0f, 0.0f, COL_EDTIOR_UI_BUTTONEDGE);
		dr->setLayer(layer);
		dr->addToMouseSelectable();
		targetState->addComponent<Button>(id, new Button(event));

		ID foreId = ID(id.getStr() + "FORE");
		targetState->addEntity(foreId);
		Transform* tran2 = targetState->addComponent<Transform>(foreId, new Transform());
		tran2->setParent(tran1);
		tran3->setParent(tran2);
		DrawRectangle* dr2 = targetState->addComponent<DrawRectangle>(foreId, new DrawRectangle());
		dr2->initialize(2.0f, 2.0f, textWidth + 8.0f, textHeight, 0.0f, 0.0f, COL_EDITOR_UI_BUTTON);
		dr2->setLayer(layer + 1);

		return id;
	}

	ID PrefabFactory::createEditorTextField(ID id, glm::vec2 position, int layer, int length, ID fontId, std::string startText)
	{
		targetState->addEntity(id);
		targetState->addEntityTag(id, ID("T_FOCUSABLE"));

		ID textId = ID(id.getStr() + "TX");
		targetState->addEntity(textId);
		Transform* textTran = targetState->addComponent<Transform>(textId, new Transform());
		DrawText* dt = targetState->addComponent<DrawText>(textId, new DrawText());
		dt->initialize(8.0f, 8.0f, startText, COL_WHITE, fontId);
		dt->setLayer(layer + 1);

		Transform* backTran = targetState->addComponent<Transform>(id, new Transform());
		DrawRectangle* dr = targetState->addComponent<DrawRectangle>(id, new DrawRectangle());
		dr->initialize(position.x, position.y, (length + 0.25f) * dt->font->maxCharWidth + 8.0f, dt->font->maxCharHeight + 8.0f, 0.0f, 0.0f, COL_EDITOR_UI_BACKGROUND);
		dr->setLayer(layer);
		dr->addToMouseSelectable();
		targetState->addComponent<EditorTextField>(id, new EditorTextField());

		textTran->setParent(backTran);

		return id;
	}

	ID PrefabFactory::createEditorFileSelector(ID id, std::string startPath, Event* selectEvent)
	{
		// ID tag = ID("T_SELECTOREDIT");

		// targetState->addEntity(id);
		// targetState->addEntityTag(id, ID("T_" + id.getStr()));
		// targetState->addEntityTag(id, tag);
		// targetState->addComponent<Transform>(id, new Transform());
		// DrawRectangle* dr1 = targetState->addComponent<DrawRectangle>(id, new DrawRectangle(config.get("WINDOW_WIDTH") / 2.0f, config.get("WINDOW_HEIGHT") / 2.0f, 850.0f, 550.0f, 425.0f, 275.0f, COL_EDITOR_UI_BACKGROUND));
		// dr1->setLayer(10);

		// ID windowId = ID(id.getStr() + "WINDOW");
		// targetState->addEntity(windowId);
		// targetState->addEntityTag(windowId, ID("T_" + id.getStr()));
		// targetState->addEntityTag(windowId, tag);
		// targetState->addComponent<Transform>(windowId, new Transform());
		// DrawRectangle* dr2 = targetState->addComponent<DrawRectangle>(windowId, new DrawRectangle(config.get("WINDOW_WIDTH") / 2.0f, (config.get("WINDOW_HEIGHT") / 2.0f) + 50.0f, 800.0f, 400.0f, 400.0f, 200.0f, COL_EDITOR_UI_FOREGROUND));
		// dr2->setLayer(11);

		// for (int i = 0; i < 40; i++)
		// {
		// 	ID itemId = ID(id.getStr() + "ITEM" + iToS(i));
		// 	targetState->addEntity(itemId);
		// 	targetState->addEntityTag(itemId, ID("T_" + id.getStr()));
		// 	targetState->addComponent<Transform>(itemId, new Transform());
		// 	DrawRectangle* dr2 = targetState->addComponent<DrawRectangle>(windowId, new DrawRectangle((config.get("WINDOW_WIDTH") / 2.0f), (config.get("WINDOW_HEIGHT") / 2.0f) + 50.0f, 400.0f, 20.0f, 0.0f, 0.0f, glm::vec4(86.0f/255.0f, 86.0f/255.0f, 94.0f/255.0f, 1.0f)));
		// 	dr2->setLayer(11);
		// }

		return id;
	}

	ID PrefabFactory::createCursor()
	{
		ID id = ID("E_CURSOR");

		if (!targetState->entityExists(id))
		{
			targetState->addEntity(id);
			targetState->addComponent<Transform>(id, new Transform());
			DrawSprite* ds = targetState->addComponent<DrawSprite>(id, new DrawSprite());
			ds->initialize(input.getMouseX(), input.getMouseY(), 1.0, 1.0, 0.0f, 0.0f, ID("SP_POINTCUR"));
			ds->pivotY = -((float)ds->sprite->height);
			targetState->addComponent<Cursor>(id, new Cursor());
		}
		else
		{
			LOG_E << "Cursor already created";
		}

		return id;
	}
}