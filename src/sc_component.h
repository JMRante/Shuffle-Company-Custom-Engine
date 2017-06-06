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

	public:
		ID entityId;
		State* state;

		Component();
		virtual ~Component() {}
		bool isType(ID id);
		bool sameTypes(Component* comp);
		void addType(ID id);

		virtual void onStateInsert();
		virtual void onStateRemove();
	};

	class Transform : public Component
	{
	private:
		glm::mat4 worldMatrix;

	public:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		// Transform* parent;
		// std::vector<Transform*> children;

		Transform();
		Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

		glm::mat4 getWorldMatrix();
		void calculateWorldMatrix();
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
		bool isVisible;

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
		float x;
		float y;
		float width;
		float height;
		float pivotX;
		float pivotY;

		glm::vec4 color;

		DrawRectangle(float x, float y, float width, float height, float pivotX, float pivotY, glm::vec4 color);
		void render(ID cameraId);
		void mouseRender(ID cameraId, unsigned int index);
		void calculateTransform();
	};

	class DrawSprite : public DrawOrtho
	{
	public:
		float x;
		float y;
		float scaleX;
		float scaleY;
		float pivotX;
		float pivotY;

		Sprite* sprite;

		DrawSprite(float x, float y, float scaleX, float scaleY, float pivotX, float pivotY, ID spriteId);
		void render(ID cameraId);
		void mouseRender(ID cameraId, unsigned int index);
		void calculateTransform();
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
		float x;
		float y;

		TextHAlign hAlignment;
		TextVAlign vAlignment;
		TextHAlign justification;
		float lineSeperation;

		glm::vec4 color;
		Font* font;

		DrawText(float x, float y, std::string text, glm::vec4 color, ID fontId);
		void render(ID cameraId);
		void mouseRender(ID cameraId, unsigned int index);
		void setText(std::string text);
		std::string getText();
		float getWidth();
		float getHeight();
		float getLineWidth(int line);
		void calculateWidth();
		void calculateHeight();
		float getDrawStartX(int line);
		float getDrawStartY();
	};
}

#endif