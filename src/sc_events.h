/*

    SHUFFLE COMPANY SOURCE CODE

    sc_events.h
    ------------------------------------



*/

#ifndef SC_EVENTS
#define SC_EVENTS

#include <vector>
#include <string>

#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "glm/ext.hpp"

#include "sc_log.h"
#include "sc_component.h"
#include "sc_utility.h"

namespace sc
{
	class Event
	{
	public:
		State* state;

		Event(State* state);
		virtual ~Event() {}
		virtual void happen() = 0;
	};

	class SaveLevelEvent : public Event
	{
	private:
		std::string levelFile;

	public:
		SaveLevelEvent(State* state, std::string levelFile);
		void happen();
	};

	class LoadLevelEvent : public Event
	{
	private:
		std::string levelFile;

	public:
		LoadLevelEvent(State* state, std::string levelFile);
		void happen();
	};

	class OpenLoadSelector : public Event
	{
	public:
		OpenLoadSelector(State* state);
		void happen();
	};

	class CloseSelector : public Event
	{
	public:
		CloseSelector(State* state);
		void happen();
	};
}

#endif