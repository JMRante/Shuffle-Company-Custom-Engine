/*

    SHUFFLE COMPANY SOURCE CODE

    sc_stage.cpp
    ------------------------------------



*/

#include "sc_stage.h"

#include <IL/il.h> 
#include <IL/ilu.h>

namespace sc
{
	Brush::Brush(int texNum)
	{
		tex_E = texNum;
		tex_N = texNum;
		tex_W = texNum;
		tex_S = texNum;
		tex_T = texNum;
		tex_B = texNum;		
	}

	Stage::Stage() : Component() 
	{
		width = 10;
		depth = 10;
		height = 10;
		stageMesh = NULL;
	}

	bool Stage::loadStage(std::string filepath)
	{
		if (!readStageFile(filepath))
		{
			LOG_E << "Error: Could not open shuff file " << filepath;
			return false;
		}

		if (!loadStageTextures())
		{
			LOG_E << "Error: Could not load the stage textures";
			return false;
		}

		if (!createStageMesh())
		{
			LOG_E << "Error: Could not build the stage mesh";
			return false;
		}

		if (!createStageModel())
		{
			LOG_E << "Error: Could not create stage model";
			return false;
		}

		return true;
	}

	bool Stage::readStageFile(std::string filepath)
	{
		std::ifstream file;
		file.open(filepath);

		if (file)
		{
			Tokenizer t(&file);
			t.addDelimiter('\r');
			t.addDelimiter('\n');
			t.addDelimiter(' ');

			t.next();

			if (t.check("word", "shuff"))
			{
				LOG_I << "Reading shuff file";
				t.next();
				if (t.check("word", "brushes"))
				{
					LOG_I << "Reading brushes";
					t.next();
					if (t.checkType("word"))
					{
						while (t.checkType("word") && !t.checkToken("stage"))
						{
							int textureNum = getTextureNum(t.getToken());

							if (textureNum == 255)
							{
								if (textures.size() >= MAX_SIMPLE_TEXTURES)
								{
									LOG_E << "Stage has too many simple textures";
									return false;
								}

								textures.push_back(t.getToken());
								textureNum = (int)(textures.size() - 1);
							}

							brushes.push_back(Brush(textureNum));
							LOG_I << "Texture: " << t.getToken();
							t.next();
							if (t.checkType("statementEnd"))
							{
								LOG_I << "End Brush";
							}
							else break;
							t.next();
						}

						if (t.check("word", "stage"))
						{
							LOG_I << "Reading stage";

							int brush;
							int x = 0;
							int y = 0;
							int z = 0;

							t.next();
							if (t.checkType("int"))
							{
								while (t.checkType("int"))
								{
									brush = atoi(t.getToken().c_str());
									brush += 1;
									LOG_I << "Brush: " << brush;
									t.next();
									if (t.checkType("int"))
									{
										x = atoi(t.getToken().c_str());
										LOG_I << "X: " << x;
										t.next();
										if (t.checkType("int"))
										{
											y = atoi(t.getToken().c_str());
											LOG_I << "Y: " << y;
											t.next();
											if (t.checkType("int"))
											{
												z = atoi(t.getToken().c_str());
												LOG_I << "Z: " << z;
												t.next();
												if (t.checkType("statementEnd"))
												{
													LOG_I << "End Stage Chunk";
													stage[x][y][z] = brush;
													if (!t.next())
													{
														file.close();
														LOG_I << "Read shuff file successfully";
														return true;
													}											
												}
												else break;
											}
											else break;
										}
										else break;
									}
									else break;
								}
							}
						}
					}
				}
			}
		}
		else
		{
			return false;
		}

		file.close();
		LOG_E << "Invalid shuff file data";
		return false;
	}

	bool Stage::loadStageTextures()
	{
		std::vector<GLuint*> dataArray;

		ILuint texIds[MAX_SIMPLE_TEXTURES];
		ilGenImages(MAX_SIMPLE_TEXTURES, texIds);

		ILboolean success;

		for (size_t i = 0; i < textures.size(); i++)
		{
			ilBindImage(texIds[i]);

			success = ilLoadImage(("Resources/StageTextures/Simple/Albedo/ST_" + textures[i] + ".png").c_str());

			if (success == IL_TRUE)
			{
				success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

				if (success == IL_TRUE)
				{
					if (ilGetInteger(IL_IMAGE_WIDTH) == SIMPLE_TEXTURE_DIM && ilGetInteger(IL_IMAGE_HEIGHT) == SIMPLE_TEXTURE_DIM)
					{
						dataArray.push_back((GLuint*)ilGetData());
					}
					else
					{
						LOG_E << "Simple stage texture has wrong dimensions. Should be 256x256";
						return false;
					}
				}
				else
				{
					LOG_E << "Error converting image to RGBA";
					return false;
				}
			}
			else
			{
				LOG_E << "Error loading simple stage texture";
				return false;
			}
		}

		assets.textureStack.pushWorld(new Texture(ID("TX_STAGE"), SIMPLE_TEXTURE_DIM, SIMPLE_TEXTURE_DIM, dataArray));
		ilDeleteImages(MAX_SIMPLE_TEXTURES, texIds);

		std::vector<ID> tempString;
		tempString.push_back(ID("TX_STAGE"));
		
		return assets.materialStack.pushWorld(new Material(ID("MA_STAGE"), NULL, NULL, NULL, &tempString, ID("SH_STAGE")));
	}

	bool Stage::createStageMesh()
	{
		std::vector<StageVertex> stageVertices;
		std::vector<int> stageIndices;

		buildStageMesh(stageVertices, stageIndices);

		stageMesh = assets.meshStack.pushWorld(new Mesh(ID("ME_STAGE"), &stageVertices, &stageIndices));

		if (stageMesh != NULL)
		{
			return true;
		}

		return false;
	}

	void Stage::buildStageMesh(std::vector<StageVertex> &stageVertices, std::vector<int> &stageIndices)
	{
		int vertCount = 0;
		StageVertex tempVert;

		LOG_D; LOG_D << "Building Stage Mesh";

		for (int i = 0; i < STAGE_WIDTH; i++)
		{
			for (int j = 0; j < STAGE_HEIGHT; j++)
			{
				for (int k = 0; k < STAGE_DEPTH; k++)
				{
					if (stage[i][j][k] != 0)
					{
						LOG_D << "(" << i << ", " << j << ", " << k << ") = " << stage[i][j][k];
						Brush* brush = &(brushes[stage[i][j][k] - 1]);
						
						//E
						if (i + 1 >= STAGE_WIDTH || stage[i + 1][j][k] == 0)
						{
							tempVert.position     = glm::vec3(i + 1.0f, j + 0.0f, k + 0.0f);
							tempVert.normal       = glm::vec3(1.0f, 0.0f, 0.0f);
							tempVert.textureCoord = glm::vec2(1.0f, 1.0f);
							tempVert.textureNum   = (float)brush->tex_E;
							stageVertices.push_back(tempVert);

							tempVert.position     = glm::vec3(i + 1.0f, j + 1.0f, k + 1.0f);
							tempVert.textureCoord = glm::vec2(0.0f, 0.0f);
							stageVertices.push_back(tempVert);

							tempVert.position     = glm::vec3(i + 1.0f, j + 0.0f, k + 1.0f);
							tempVert.textureCoord = glm::vec2(0.0f, 1.0f);
							stageVertices.push_back(tempVert);

							tempVert.position     = glm::vec3(i + 1.0f, j + 1.0f, k + 0.0f);
							tempVert.textureCoord = glm::vec2(1.0f, 0.0f);
							stageVertices.push_back(tempVert);

							stageIndices.push_back(vertCount);
							stageIndices.push_back(vertCount + 1);
							stageIndices.push_back(vertCount + 2);

							stageIndices.push_back(vertCount);
							stageIndices.push_back(vertCount + 3);
							stageIndices.push_back(vertCount + 1);

							vertCount += 4;
						}

						//W
						if (i - 1 <= -1 || stage[i - 1][j][k] == 0)
						{
							tempVert.position     = glm::vec3(i + 0.0f, j + 0.0f, k + 1.0f);
							tempVert.normal       = glm::vec3(-1.0f, 0.0f, 0.0f);
							tempVert.textureCoord = glm::vec2(1.0f, 1.0f);
							tempVert.textureNum   = (float)brush->tex_W;
							stageVertices.push_back(tempVert);

							tempVert.position     = glm::vec3(i + 0.0f, j + 1.0f, k + 0.0f);
							tempVert.textureCoord = glm::vec2(0.0f, 0.0f);
							stageVertices.push_back(tempVert);

							tempVert.position     = glm::vec3(i + 0.0f, j + 0.0f, k + 0.0f);
							tempVert.textureCoord = glm::vec2(0.0f, 1.0f);
							stageVertices.push_back(tempVert);

							tempVert.position     = glm::vec3(i + 0.0f, j + 1.0f, k + 1.0f);
							tempVert.textureCoord = glm::vec2(1.0f, 0.0f);
							stageVertices.push_back(tempVert);

							stageIndices.push_back(vertCount);
							stageIndices.push_back(vertCount + 1);
							stageIndices.push_back(vertCount + 2);

							stageIndices.push_back(vertCount);
							stageIndices.push_back(vertCount + 3);
							stageIndices.push_back(vertCount + 1);

							vertCount += 4;
						}

						//T
						if (j + 1 >= STAGE_HEIGHT || stage[i][j + 1][k] == 0)
						{
							tempVert.position     = glm::vec3(i + 0.0f, j + 1.0f, k + 1.0f);
							tempVert.normal       = glm::vec3(0.0f, 1.0f, 0.0f);
							tempVert.textureCoord = glm::vec2(1.0f, 1.0f);
							tempVert.textureNum   = (float)brush->tex_S;
							stageVertices.push_back(tempVert);

							tempVert.position     = glm::vec3(i + 1.0f, j + 1.0f, k + 0.0f);
							tempVert.textureCoord = glm::vec2(0.0f, 0.0f);
							stageVertices.push_back(tempVert);

							tempVert.position     = glm::vec3(i + 0.0f, j + 1.0f, k + 0.0f);
							tempVert.textureCoord = glm::vec2(1.0f, 0.0f);
							stageVertices.push_back(tempVert);

							tempVert.position     = glm::vec3(i + 1.0f, j + 1.0f, k + 1.0f);
							tempVert.textureCoord = glm::vec2(0.0f, 1.0f);
							stageVertices.push_back(tempVert);

							stageIndices.push_back(vertCount);
							stageIndices.push_back(vertCount + 1);
							stageIndices.push_back(vertCount + 2);

							stageIndices.push_back(vertCount);
							stageIndices.push_back(vertCount + 3);
							stageIndices.push_back(vertCount + 1);

							vertCount += 4;
						}

						//B
						// if (j - 1 <= -1 || stage[i][j - 1][k] == 0)
						// {
						// 	tempVert.position     = glm::vec3(i + 1.0f, j + 0.0f, k + 0.0f);
						// 	tempVert.normal       = glm::vec3(0.0f, -1.0f, 0.0f);
						// 	tempVert.textureCoord = glm::vec2(1.0f, 1.0f);
						// 	tempVert.textureNum   = (float)brush->tex_N;
						// 	stageVertices.push_back(tempVert);

						// 	tempVert.position     = glm::vec3(i + 0.0f, j + 0.0f, k + 1.0f);
						// 	tempVert.textureCoord = glm::vec2(0.0f, 0.0f);
						// 	stageVertices.push_back(tempVert);

						// 	tempVert.position     = glm::vec3(i + 0.0f, j + 0.0f, k + 0.0f);
						// 	tempVert.textureCoord = glm::vec2(0.0f, 1.0f);
						// 	stageVertices.push_back(tempVert);

						// 	tempVert.position     = glm::vec3(i + 1.0f, j + 0.0f, k + 1.0f);
						// 	tempVert.textureCoord = glm::vec2(1.0f, 0.0f);
						// 	stageVertices.push_back(tempVert);

						// 	stageIndices.push_back(vertCount);
						// 	stageIndices.push_back(vertCount + 1);
						// 	stageIndices.push_back(vertCount + 2);

						// 	stageIndices.push_back(vertCount);
						// 	stageIndices.push_back(vertCount + 3);
						// 	stageIndices.push_back(vertCount + 1);

						// 	vertCount += 4;					
						// }

						//S
						if (k + 1 >= STAGE_DEPTH || stage[i][j][k + 1] == 0)
						{
							tempVert.position     = glm::vec3(i + 0.0f, j + 0.0f, k + 1.0f);
							tempVert.normal       = glm::vec3(0.0f, 0.0f, 1.0f);
							tempVert.textureCoord = glm::vec2(1.0f, 1.0f);
							tempVert.textureNum   = (float)brush->tex_T;
							stageVertices.push_back(tempVert);

							tempVert.position     = glm::vec3(i + 1.0f, j + 1.0f, k + 1.0f);
							tempVert.textureCoord = glm::vec2(0.0f, 0.0f);
							stageVertices.push_back(tempVert);

							tempVert.position     = glm::vec3(i + 0.0f, j + 1.0f, k + 1.0f);
							tempVert.textureCoord = glm::vec2(1.0f, 0.0f);
							stageVertices.push_back(tempVert);

							tempVert.position     = glm::vec3(i + 1.0f, j + 0.0f, k + 1.0f);
							tempVert.textureCoord = glm::vec2(0.0f, 1.0f);
							stageVertices.push_back(tempVert);

							stageIndices.push_back(vertCount);
							stageIndices.push_back(vertCount + 1);
							stageIndices.push_back(vertCount + 2);

							stageIndices.push_back(vertCount);
							stageIndices.push_back(vertCount + 3);
							stageIndices.push_back(vertCount + 1);

							vertCount += 4;
						}

						//N
						// if (k - 1 <= -1 || stage[i][j][k - 1] == 0)
						// {
						// 	tempVert.position     = glm::vec3(i + 0.0f, j + 0.0f, k + 0.0f);
						// 	tempVert.normal       = glm::vec3(0.0f, 0.0f, -1.0f);
						// 	tempVert.textureCoord = glm::vec2(0.0f, 1.0f);
						// 	tempVert.textureNum   = (float)brush->tex_B;
						// 	stageVertices.push_back(tempVert);

						// 	tempVert.position     = glm::vec3(i + 1.0f, j + 1.0f, k + 0.0f);
						// 	tempVert.textureCoord = glm::vec2(1.0f, 0.0f);
						// 	stageVertices.push_back(tempVert);

						// 	tempVert.position     = glm::vec3(i + 1.0f, j + 0.0f, k + 0.0f);
						// 	tempVert.textureCoord = glm::vec2(1.0f, 1.0f);
						// 	stageVertices.push_back(tempVert);

						// 	tempVert.position     = glm::vec3(i + 0.0f, j + 1.0f, k + 0.0f);
						// 	tempVert.textureCoord = glm::vec2(0.0f, 0.0f);
						// 	stageVertices.push_back(tempVert);

						// 	stageIndices.push_back(vertCount);
						// 	stageIndices.push_back(vertCount + 1);
						// 	stageIndices.push_back(vertCount + 2);

						// 	stageIndices.push_back(vertCount);
						// 	stageIndices.push_back(vertCount + 3);
						// 	stageIndices.push_back(vertCount + 1);

						// 	vertCount += 4;
						// }
					}
				}
			}
		}
	}

	bool Stage::createStageModel()
	{
		if (assets.modelStack.pushWorld(new Model(ID("MO_STAGE"), ID("ME_STAGE"), ID("MA_STAGE"))) != NULL)
		{
			return true;			
		}

		return false;
	}

	void Stage::updateStageMesh()
	{
		std::vector<StageVertex> stageVertices;
		std::vector<int> stageIndices;

		buildStageMesh(stageVertices, stageIndices);

		stageMesh->updateStage(&stageVertices, &stageIndices);		
	}

	int Stage::getTextureX(int textureNum)
	{
		if (textureNum < 128)
		{
			return (textureNum * SIMPLE_TEXTURE_DIM) % STAGE_TEXTURE_DIM;
		}

		if (textureNum < 192)
		{
			return (textureNum * SIMPLE_TEXTURE_DIM) % (STAGE_TEXTURE_DIM / 2);
		}

		if (textureNum == 192)
		{
			return 2186;
		}

		if (textureNum == 193)
		{
			return 3072;
		}

		return -1;
	}

	int Stage::getTextureY(int textureNum)
	{
		if (textureNum < 192)
		{
			return ((textureNum * SIMPLE_TEXTURE_DIM) / STAGE_TEXTURE_DIM) * SIMPLE_TEXTURE_DIM;
		}

		if (textureNum == 192 || textureNum == 193)
		{
			return 2048;
		}

		return -1;
	}

	float Stage::getTextureUMin(int textureNum)
	{
		// return (float)getTextureX(textureNum) / (float)STAGE_TEXTURE_DIM;
		return 0.0f;
	}

	float Stage::getTextureVMin(int textureNum)
	{
		// return (float)getTextureY(textureNum) / (float)STAGE_TEXTURE_DIM;
		return 0.0f;
	}

	float Stage::getTextureUMax(int textureNum)
	{
		// return getTextureUMin(textureNum) + ((float)SIMPLE_TEXTURE_DIM / (float)STAGE_TEXTURE_DIM);
		return 0.5f;
	}

	float Stage::getTextureVMax(int textureNum)
	{
		// return getTextureVMin(textureNum) + ((float)SIMPLE_TEXTURE_DIM / (float)STAGE_TEXTURE_DIM);
		return 0.5f;
	}

	int Stage::getTextureNum(std::string textureName)
	{
		size_t i = 0;

		while (i < textures.size())
		{
			if (textures[i].compare(textureName) == 0)
			{
				return (int)i;
			}

			i++;
		}

		return 255;
	}

	int Stage::getWidth()
	{
		return width;
	}

	int Stage::getDepth()
	{
		return depth;
	}

	int Stage::getHeight()
	{
		return height;
	}

	void Stage::drawBrush(std::vector<glm::ivec3>* slots, int brush)
	{
		for (auto it = slots->begin(); it != slots->end(); it++) 
		{
			stage[(*it).x][(*it).y][(*it).z] = brush + 1;
		}
	}
}