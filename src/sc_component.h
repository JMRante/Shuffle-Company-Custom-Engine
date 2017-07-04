/*

    SHUFFLE COMPANY SOURCE CODE

    sc_component.h
    ------------------------------------



*/

#ifndef SC_COMPONENT
#define SC_COMPONENT

#include <vector>
#include <algorithm>
#include <string>

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "glm/ext.hpp"

#include "sc_log.h"
#include "sc_assets.h"
#include "sc_utility.h"

namespace sc
{
	class State;

	class Component
	{
	private:
		std::vector<ID> types;

	protected:
		bool isActive;

	public:
		ID entityId;
		State* state;

		Component();
		virtual ~Component() {}

		virtual void create() {}
		virtual void destroy() {}

		bool isType(ID id);
		bool sameTypes(Component* comp);
		void addType(ID id);

		virtual void onStateInsert() {};
		virtual void onStateRemove() {};

		virtual bool getActive();
		virtual void setActive(bool set);
	};

	class Transform : public Component
	{
	private:
		Transform* parent;
		std::vector<Transform*> children;
		bool dirty;

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		glm::mat4 matrix;

	public:
		bool useParentTransform;
		bool useParentScale;
		bool useParentRotation;

		Transform();
		Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

		void onStateInsert();
		void onStateRemove();

		bool getActive();
		void setActive(bool set);

		glm::mat4 calculate();
		glm::mat4 getMatrix();
		void setParent(Transform* newParent);
		void removeParent();

		glm::vec3 getPos();
		float getPosX();
		float getPosY();
		float getPosZ();
		void setPos(glm::vec3 pos);
		void setPosX(float x);
		void setPosY(float y);
		void setPosZ(float z);

		glm::vec3 getRot();
		float getRotX();
		float getRotY();
		float getRotZ();
		void setRot(glm::vec3 rot);
		void setRotX(float x);
		void setRotY(float y);
		void setRotZ(float z);

		glm::vec3 getSca();
		float getScaX();
		float getScaY();
		float getScaZ();
		void setSca(glm::vec3 sca);
		void setScaX(float x);
		void setScaY(float y);
		void setScaZ(float z);
	};

	class Camera : public Component
	{
	private:
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		glm::mat4 orthoMatrix;

		float fov;
		float aspectRatio;
		float near;
		float far;

		glm::vec3 forward;
		glm::vec3 up;
		glm::vec3 side;

	public:
		Camera();
		Camera(float near, float far);

		glm::mat4 getProjectionMatrix();
		void calculateProjectionMatrix();

		glm::mat4 getViewMatrix();
		void calculateViewMatrix();

		glm::mat4 getOrthoMatrix();
		void calculateOrthoMatrix();
		
		glm::vec3 getForward();
		glm::vec3 getUp();
		glm::vec3 getSide();
	};	

	class Draw : public Component
	{
	protected:
		bool isMouseSelectable;

	public:
		Draw();

		virtual void addToMouseSelectable();
		virtual void removeFromMouseSelectable();
	};

	class DrawModel : public Draw
	{
	public:
		Model* model;

		DrawModel();
		DrawModel(ID modelId);
		void render(ID cameraId);
		void mouseRender(ID cameraId, unsigned int index);
		void stageMouseRender(ID cameraId);

		void onStateInsert();
		void onStateRemove();

		void addToMouseSelectable();
		void removeFromMouseSelectable();
	};

	class DrawOrtho : public Draw
	{
	protected:
		int layer;

	public:
		DrawOrtho();
		virtual void render(ID cameraId);
		virtual void mouseRender(ID cameraId, unsigned int index);
		void setLayer(int layer);

		void onStateInsert();
		void onStateRemove();

		void addToMouseSelectable();
		void removeFromMouseSelectable();

		static bool compare(DrawOrtho* l, DrawOrtho* r);
	};

	class DrawRectangle : public DrawOrtho
	{
	public:
		float pivotX;
		float pivotY;

		glm::vec4 color;

		DrawRectangle();
		void initialize(float x, float y, float width, float height, float pivotX, float pivotY, glm::vec4 color);
		void render(ID cameraId);
		void mouseRender(ID cameraId, unsigned int index);
	};

	class DrawSprite : public DrawOrtho
	{
	public:
		float pivotX;
		float pivotY;
		float scaleX;
		float scaleY;

		Sprite* sprite;

		DrawSprite();
		void initialize(float x, float y, float scaleX, float scaleY, float pivotX, float pivotY, ID spriteId);
		void render(ID cameraId);
		void mouseRender(ID cameraId, unsigned int index);
	};

	enum TextHAlign {left, center, right};
	enum TextVAlign {top, middle, bottom};
	class DrawText : public DrawOrtho
	{
	private:
		std::string text;
		std::vector<float> lineWidths;
		float width;
		float height;

	public:
		TextHAlign hAlignment;
		TextVAlign vAlignment;
		TextHAlign justification;
		float lineSeperation;

		glm::vec4 color;
		Font* font;

		DrawText();
		void initialize(float x, float y, std::string text, glm::vec4 color, ID fontId);
		void render(ID cameraId);
		void mouseRender(ID cameraId, unsigned int index);
		void setText(std::string text);
		std::string getText();
		float getWidth();
		float getHeight();
		float getLineWidth(int line);
		void calculateWidth();
		void calculateHeight();
		float getDrawOffsetX(int line);
		float getDrawOffsetY();
	};

	class EditorOperation
	{
	public:
		State* state;

		EditorOperation();
		virtual void operate() = 0;
		virtual void reverse() = 0;
	};

	class SetBrush : public EditorOperation
	{
	private:
		int newBrush;
		int previousBrush;
		glm::ivec3 position;

	public:
		SetBrush(int newBrush, glm::ivec3 position);
		void operate();
		void reverse();
	};

	class EditorOperationManager : public Component
	{
	private:
		size_t maxOperations;
		size_t currentOperation;
		std::vector<EditorOperation*> operations;

	public:
		EditorOperationManager(size_t maxOperations);
		void doOperation(EditorOperation* operation);
		void undoOperation();
		void redoOperation();
		void clearOperations();
	};
}

#endif