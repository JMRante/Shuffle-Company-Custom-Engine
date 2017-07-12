/*

    SHUFFLE COMPANY SOURCE CODE

    sc_stage.h
    ------------------------------------

	Sample Stage File:

	NAME:"Crate's Landing";
	W:20;
	H:10;
	D:30;
	TEX:RED,BLUE;
	BRUSH:(0)(1)(0,1);
	STAGE:(0,4)(2,45);

*/

#ifndef SC_STAGE
#define SC_STAGE

#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"

#include "sc_log.h"
#include "sc_assets.h"
#include "sc_tokenizer.h"
#include "sc_utility.h"
#include "sc_component.h"
#include "sc_id.h"

#define STAGE_WIDTH 200
#define STAGE_DEPTH 200
#define STAGE_HEIGHT 10

#define MAX_SIMPLE_TEXTURES 200
#define MAX_AUTO_TEXTURES 20

#define STAGE_TEXTURE_DIM 4096
#define SIMPLE_TEXTURE_DIM 256

namespace sc
{
	class Brush
	{
	public:
		int tex_E;
		int tex_W;
		int tex_S;
		int tex_T;

		Brush(int texNum);
		Brush(int texTop, int texBottom);
		Brush(int texTop, int texSouth, int texWest, int texEast);
	};

	class Stage : public Component
	{
	private:
		std::vector<std::string> textures;
		std::vector<Brush*> brushes;
		int stage[STAGE_WIDTH * STAGE_HEIGHT * STAGE_DEPTH] = {};
		int width;
		int depth;
		int height;
		Mesh* stageMesh;

	public:
		std::string title;

		Stage();

		bool readStageFile(std::string filepath);
		bool parseTitle(Tokenizer &t);
		bool parseDimensions(Tokenizer &t);
		bool parseTextures(Tokenizer &t);
		bool parseBrushes(Tokenizer &t);
		bool parseStage(Tokenizer &t);
		bool writeStageFile(std::string filepath);

		bool createStageMesh();
		void buildStageMesh(std::vector<StageVertex> &stageVertices, std::vector<int> &stageIndices);
		void updateStageMesh();

		bool getDefaultStageTextures();
		bool loadStageMaterial();
		int getTextureNum(std::string textureName);

		int getWidth();
		int getDepth();
		int getHeight();

		int get(int x, int y, int z);
		void set(int x, int y, int z, int brush);
	};
}

#endif