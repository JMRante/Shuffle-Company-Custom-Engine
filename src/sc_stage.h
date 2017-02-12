/*

    SHUFFLE COMPANY SOURCE CODE

    sc_stage.h
    ------------------------------------



*/

#ifndef SC_STAGE
#define SC_STAGE

#include <vector>
#include <fstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"

#include "sc_log.h"
#include "sc_assets.h"
#include "sc_tokenizer.h"

namespace sc
{
	class Brush
	{
	public:
		std::string mat_E;
		std::string mat_N;
		std::string mat_W;
		std::string mat_S;
		std::string mat_T;
		std::string mat_B;

		Brush(std::string mat);
	};

	class Stage
	{
	private:
		sc::Model* model;
		std::vector<Brush> brushes;

	public:
		bool loadStage(std::string filepath);
		sc::Model* getModel();
	};
}

#endif