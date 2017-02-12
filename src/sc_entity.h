/*

    SHUFFLE COMPANY SOURCE CODE

    sc_entity.h
    ------------------------------------



*/

#ifndef SC_ENTITY
#define SC_ENTITY

#include <vector>
#include <string>
#include <algorithm>

#include "sc_log.h"
#include "sc_component.h"
#include "sc_behaviors.h"

namespace sc
{
	class EntityManager
	{
	private:
		std::vector<std::string> entities;

		std::vector<sc::Transform> transformPool;
		std::vector<sc::Camera> cameraPool;
		std::vector<sc::Draw> drawPool;
		std::vector<sc::Behavior*> behaviorPool;

	public:
		void copy(EntityManager* otherEM);
		bool addEntity(std::string id);
		bool entityExists(std::string id);
		bool removeEntity(std::string id);

		bool addCamera(std::string id, sc::Camera camera);
		bool addDraw(std::string id, sc::Draw draw);
		bool addBehavior(std::string id, sc::Behavior* behavior);

		bool removeCamera(std::string id);
		bool removeDraw(std::string id);
		bool removeBehavior(std::string id);

		sc::Transform* getTransform(std::string id);
		sc::Camera* getCamera(std::string id);
		sc::Draw* getDraw(std::string id);
		sc::Behavior* getBehavior(std::string id);

		std::vector<sc::Transform>::iterator getTransformPoolBegin();
		std::vector<sc::Camera>::iterator getCameraPoolBegin();
		std::vector<sc::Draw>::iterator getDrawPoolBegin();
		std::vector<sc::Behavior*>::iterator getBehaviorPoolBegin();

		std::vector<sc::Transform>::iterator getTransformPoolEnd();
		std::vector<sc::Camera>::iterator getCameraPoolEnd();
		std::vector<sc::Draw>::iterator getDrawPoolEnd();
		std::vector<sc::Behavior*>::iterator getBehaviorPoolEnd();
	};
}

#endif