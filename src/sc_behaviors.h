/*

    SHUFFLE COMPANY SOURCE CODE

    sc_behaviors.h
    ------------------------------------



*/

#ifndef SC_GAMEELEMENTS
#define SC_GAMEELEMENTS

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

namespace sc
{
	class World;

	class Behavior : public sc::Component
	{
	public:
		Behavior();
		virtual void update(sc::World* pastWorld, sc::World* nextWorld) = 0;
	};

	class DebugCamera : public Behavior
	{
	private:
		float moveSpeed;
		float mouseSpeed;
		float yaw;
		float pitch;

	public:
		DebugCamera(float moveSpeed, float mouseSpeed);

		void update(sc::World* pastWorld, sc::World* nextWorld);
	};
}

#endif