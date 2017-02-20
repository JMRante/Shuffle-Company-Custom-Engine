/*

    SHUFFLE COMPANY SOURCE CODE

    sc_ui.h
    ------------------------------------



*/

#ifndef SC_UI
#define SC_UI

#include <vector>
#include <string>

#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"

#include "sc_log.h"
#include "sc_component.h"

namespace sc
{
	class UIDraw : public sc::Component
	{
	public:
		bool isVisible;
		sc::Transform* transform;

		UIDraw(sc::ComponentType type, sc::Transform* transform, bool isVisible);
	};

	class UIDrawRectangle : public UIDraw
	{
	private:
		float x;
		float y;
		float width;
		float height;
	public:
		glm::vec4 color;

		UIDrawRectangle(sc::Transform* transform, bool isVisible, float x, float y, float width, float height, glm::vec4 color);
		void changeTransform(float x, float y, float width, float height);
		void calculateTransform();
	};
}

#endif