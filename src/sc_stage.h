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
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"

#include "sc_log.h"
#include "sc_assets.h"
#include "sc_tokenizer.h"
#include "sc_utility.h"
#include "sc_component.h"

#define STAGE_WIDTH 200
#define STAGE_DEPTH 200
#define STAGE_HEIGHT 100

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
		int tex_N;
		int tex_W;
		int tex_S;
		int tex_T;
		int tex_B;

		Brush(int texNum);
	};

	class Stage : public Component
	{
	private:
		std::vector<std::string> textures;
		std::vector<Brush> brushes;
		int stage[STAGE_WIDTH][STAGE_HEIGHT][STAGE_DEPTH] = {};
		int width;
		int depth;
		int height;
		Mesh* stageMesh;

	public:
		Stage();

		bool loadStage(std::string filepath);
		bool readStageFile(std::string filepath);
		bool loadStageTextures();
		bool createStageMesh();
		void buildStageMesh(std::vector<StageVertex> &stageVertices, std::vector<int> &stageIndices);
		bool createStageModel();

		void updateStageMesh();

		int getTextureX(int textureNum);
		int getTextureY(int textureNum);

		float getTextureUMin(int textureNum);
		float getTextureVMin(int textureNum);
		float getTextureUMax(int textureNum);
		float getTextureVMax(int textureNum);

		int getTextureNum(std::string textureName);

		int getWidth();
		int getDepth();
		int getHeight();

		void drawBrush(std::vector<glm::ivec3>* slots, int brush);
	};
}

#endif