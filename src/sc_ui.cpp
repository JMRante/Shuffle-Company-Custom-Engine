/*

    SHUFFLE COMPANY SOURCE CODE

    sc_ui.cpp
    ------------------------------------



*/

#include "sc_ui.h"

namespace sc
{
	UIDraw::UIDraw(std::string id, sc::Transform* transform, bool isVisible) : sc::Component(id)
	{
		this->isVisible = isVisible;
		this->transform = transform;
	}

	UIDrawRectangle::UIDrawRectangle(sc::Transform* transform, bool isVisible, float x, float y, float width, float height, glm::vec4 color) : UIDraw(UIDrawRectangle, transform, isVisible)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->color = color;

		calculateTransform();
	}

	void UIDrawRectangle::changeTransform(float x, float y, float width, float height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;

		calculateTransform();	
	}

	void UIDrawRectangle::calculateTransform()
	{
		transform->setScale(glm::vec3(width, height, 0.0f));
		transform->setPosition(glm::vec3(x, y, 0.0f));
	}
}