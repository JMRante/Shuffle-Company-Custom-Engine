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
		unsigned char tex_E;
		unsigned char tex_N;
		unsigned char tex_W;
		unsigned char tex_S;
		unsigned char tex_T;
		unsigned char tex_B;

		Brush(unsigned char texNum);
	};

	class Stage : public Component
	{
	private:
		std::vector<std::string> textures;
		std::vector<Brush> brushes;
		unsigned char stage[STAGE_WIDTH][STAGE_DEPTH][STAGE_HEIGHT] = {};
		int width;
		int depth;
		int height;

	public:
		Stage();

		bool loadStage(std::string filepath);
		bool readStageFile(std::string filepath);
		bool loadStageTextures();
		bool buildStageMesh();
		bool createStageModel();

		int getTextureX(unsigned char textureNum);
		int getTextureY(unsigned char textureNum);

		float getTextureUMin(unsigned char textureNum);
		float getTextureVMin(unsigned char textureNum);
		float getTextureUMax(unsigned char textureNum);
		float getTextureVMax(unsigned char textureNum);

		unsigned char getTextureNum(std::string textureName);

		int getWidth();
		int getDepth();
		int getHeight();
	};
}

#endif