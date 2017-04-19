/*

    SHUFFLE COMPANY SOURCE CODE

    sc_component.h
    ------------------------------------



*/

#ifndef SC_COMPONENT
#define SC_COMPONENT

#include <vector>
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

	class DrawModel : public Component
	{
	public:
		Model* model;
		bool isVisible;

		DrawModel();
		DrawModel(ID modelId, bool isVisible);
		void render(ID cameraId);
	};

	class OrthoDraw : public Component
	{
	protected:
		int layer;

	public:
		OrthoDraw();
		virtual void render(ID cameraId);
		void setLayer(int i);

		void onStateInsert();
		void onStateRemove();
	};

	class DrawRectangle : public OrthoDraw
	{
	public:
		float x;
		float y;
		float width;
		float height;
		float pivotX;
		float pivotY;

		glm::vec4 color;
		bool isVisible;

		DrawRectangle(float x, float y, float width, float height, float pivotX, float pivotY, glm::vec4 color, bool isVisible);
		//void render(ID cameraId);
		void calculateTransform();
	};

	class DrawSprite : public OrthoDraw
	{
	public:
		float x;
		float y;
		float scaleX;
		float scaleY;
		float pivotX;
		float pivotY;

		Sprite* sprite;
		bool isVisible;

		DrawSprite(float x, float y, float scaleX, float scaleY, float pivotX, float pivotY, ID spriteId, bool isVisible);
		//void render(ID cameraId);
		void calculateTransform();
	};

	enum TextHAlign {left, center, right};
	enum TextVAlign {top, middle, bottom};
	class DrawText : public OrthoDraw
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
		bool isVisible;

		DrawText(float x, float y, std::string text, glm::vec4 color, ID fontId);
		//void render(ID cameraId);
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