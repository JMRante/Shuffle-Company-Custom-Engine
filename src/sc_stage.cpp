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
		tex_W = texNum;
		tex_S = texNum;
		tex_T = texNum;
	}

	Brush::Brush(int texTop, int texBottom)
	{
		tex_E = texBottom;
		tex_W = texBottom;
		tex_S = texBottom;
		tex_T = texTop;
	}

	Brush::Brush(int texTop, int texSouth, int texWest, int texEast)
	{
		tex_E = texEast;
		tex_W = texWest;
		tex_S = texSouth;
		tex_T = texTop;
	}

	Stage::Stage() : Component() 
	{
		width = 10;
		depth = 10;
		height = 10;
		stageMesh = NULL;

		if (getDefaultStageTextures())
		{
			if (loadStageTextures())
			{
				if (createStageMesh())
				{
					if (createStageModel())
					{
						brushes.push_back(new Brush(1));
					}
					else
					{
						LOG_E << "Error: Could not create stage model";
					}
				}
				else
				{
					LOG_E << "Error: Could not build the stage mesh";
				}
			}
			else
			{
				LOG_E << "Error: Could not load the stage textures to the GPU";
			}
		}
		else
		{
			LOG_E << "Error: Could not get the default stage textures";
		}
	}

	bool Stage::readStageFile(std::string filepath)
	{
		std::ifstream file;
		file.open(filepath);

		LOG_D << "Parsing Level File"; LOG_FLUSH;
		if (file)
		{
			Tokenizer t(&file);

			LOG_D << "Reading Name"; LOG_FLUSH;
			if (parseName(t))
			{
				LOG_D << "Reading Dimensions"; LOG_FLUSH;
				if (parseDimensions(t))
				{
					LOG_D << "Reading Textures"; LOG_FLUSH;
					if (parseTextures(t))
					{
						LOG_D << "Reading Brushes"; LOG_FLUSH;
						if (parseBrushes(t))
						{
							LOG_D << "Reading Stage"; LOG_FLUSH;
							if (parseStage(t))
							{
								return true;
							}
						}
					}
				}
			}
		}
		else
		{
			LOG_E << "Failed to load file " << filepath;
			return false;
		}

		file.close();
		LOG_E << "Invalid shuff file data";
		return false;
	}

	bool Stage::parseName(Tokenizer &t)
	{
		t.next();

		if (t.check(Token::word, "NAME"))
		{
			t.next();

			if (t.check(Token::colon))
			{
				t.next();

				if (t.check(Token::string))
				{
					name = t.getToken();
					t.next();

					if (t.check(Token::semicolon))
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	bool Stage::parseDimensions(Tokenizer &t)
	{
		t.next();

		if (t.check(Token::word, "W"))
		{
			t.next();

			if (t.check(Token::colon))
			{
				t.next();

				if (t.check(Token::integer))
				{
					width = atoi(t.getToken().c_str());
					t.next();

					if (t.check(Token::semicolon));
					{
						t.next();

						if (t.check(Token::word, "H"))
						{
							t.next();

							if (t.check(Token::colon))
							{
								t.next();

								if (t.check(Token::integer))
								{
									height = atoi(t.getToken().c_str());
									t.next();

									if (t.check(Token::semicolon));
									{
										t.next();

										if (t.check(Token::word, "D"))
										{
											t.next();

											if (t.check(Token::colon))
											{
												t.next();

												if (t.check(Token::integer))
												{
													depth = atoi(t.getToken().c_str());
													t.next();

													if (t.check(Token::semicolon));
													{
														return true;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		return false;
	}

	bool Stage::parseTextures(Tokenizer &t)
	{
		textures.clear();
		t.next();

		if (t.check(Token::word, "TEX"))
		{
			t.next();

			if (t.check(Token::colon))
			{
				t.next();

				while (t.check(Token::word))
				{
					textures.push_back(t.getToken());
					t.next();

					if (t.check(Token::comma))
					{
						t.next();
					}
					else if (t.check(Token::semicolon))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
		}

		return false;
	}

	bool Stage::parseBrushes(Tokenizer &t)
	{
		brushes.clear();
		t.next();

		if (t.check(Token::word, "BRUSH"))
		{
			t.next();

			if (t.check(Token::colon))
			{
				t.next();

				while (t.check(Token::paranl))
				{
					bool complete = false;
					t.next();

					if (t.check(Token::integer))
					{
						int top = atoi(t.getToken().c_str());
						t.next();

						if (t.check(Token::comma))
						{
							t.next();

							if (t.check(Token::integer))
							{
								int bottom = atoi(t.getToken().c_str());
								t.next();

								if (t.check(Token::comma))
								{
									t.next();

									if (t.check(Token::integer))
									{
										int west = atoi(t.getToken().c_str());
										t.next();

										if (t.check(Token::comma))
										{
											t.next();

											if (t.check(Token::integer))
											{
												int east = atoi(t.getToken().c_str());
												t.next();

												if (t.check(Token::paranr))
												{
													brushes.push_back(new Brush(top, bottom, west, east));
													t.next();
													complete = true;
												}
											}
										}
									}
								}
								else if (t.check(Token::paranr))
								{
									brushes.push_back(new Brush(top, bottom));
									t.next();
									complete = true;
								}
							}
						}
						else if (t.check(Token::paranr))
						{
							brushes.push_back(new Brush(top));
							t.next();
							complete = true;
						}
					}

					if (!complete)
					{
						return false;
					}
				}

				if (t.check(Token::semicolon))
				{
					return true;
				}
			}
		}

		return false;
	}

	bool Stage::parseStage(Tokenizer &t)
	{
		std::fill(stage, stage + (STAGE_WIDTH * STAGE_HEIGHT * STAGE_DEPTH), 0);
		int i = 0;
		t.next();

		if (t.check(Token::word, "STAGE"))
		{
			t.next();

			if (t.check(Token::colon))
			{
				t.next();

				while (t.check(Token::paranl))
				{
					bool complete = false;
					t.next();

					if (t.check(Token::integer))
					{
						int brush = atoi(t.getToken().c_str());
						t.next();

						if (t.check(Token::comma))
						{
							t.next();

							if (t.check(Token::integer))
							{
								int count = atoi(t.getToken().c_str());
								t.next();

								if (t.check(Token::paranr))
								{
									for (int j = 0; j < count; j++)
									{
										stage[i] = brush;
										i++;
									}

									t.next();
									complete = true;
								}
							}
						}
					}

					if (!complete)
					{
						return false;
					}
				}

				if (t.check(Token::semicolon))
				{
					return true;
				}
			}
		}

		return false;
	}

	bool Stage::writeStageFile(std::string filepath)
	{
		std::ofstream file;
		file.open(filepath);

		file << "NAME:\"" << name << "\";\n";
		file << "W:" << width << ";\n";
		file << "H:" << height << ";\n";
		file << "D:" << depth << ";\n";

		file << "TEX:";
		for (size_t i = 0; i < textures.size(); i++)
		{
			file << textures[i] << ",";
		}
		file << ";\n";

		file << "BRUSH:";
		for (size_t i = 0; i < brushes.size(); i++)
		{
			file << "(";

			if (brushes[i]->tex_T == brushes[i]->tex_S &&
				brushes[i]->tex_T == brushes[i]->tex_W &&
				brushes[i]->tex_T == brushes[i]->tex_E)
			{
				file << brushes[i]->tex_T;
			}
			else if (brushes[i]->tex_T != brushes[i]->tex_S &&
					 brushes[i]->tex_S == brushes[i]->tex_W &&
					 brushes[i]->tex_S == brushes[i]->tex_E)
			{
				file << brushes[i]->tex_T;
				file << ",";
				file << brushes[i]->tex_S;
			}
			else
			{
				file << brushes[i]->tex_T;
				file << ",";
				file << brushes[i]->tex_S;
				file << ",";
				file << brushes[i]->tex_W;
				file << ",";
				file << brushes[i]->tex_E;
			}

			file << ")";
		}
		file << ";\n";

		int j = 0;
		file << "STAGE:";		
		while (j < STAGE_WIDTH * STAGE_HEIGHT * STAGE_DEPTH)
		{
			int brushLineCount = 0;
			int currentBrush = stage[j];

			while (stage[j] == currentBrush)
			{
				brushLineCount++;
				j++;
			}

			file << "(" << currentBrush << "," << brushLineCount << ")";
		}
		file << ";";

		file.close();

		return true;
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

		for (int i = 0; i < STAGE_WIDTH; i++)
		{
			for (int j = 0; j < STAGE_HEIGHT; j++)
			{
				for (int k = 0; k < STAGE_DEPTH; k++)
				{
					if (get(i, j, k) != 0)
					{
						Brush* brush = brushes[get(i, j, k) - 1];
						
						//E
						if (i + 1 >= STAGE_WIDTH || get(i + 1, j, k) == 0)
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
						if (i - 1 <= -1 || get(i - 1, j, k) == 0)
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
						if (j + 1 >= STAGE_HEIGHT || get(i, j + 1, k) == 0)
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
						if (k + 1 >= STAGE_DEPTH || get(i, j, k + 1) == 0)
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

	void Stage::updateStageMesh()
	{
		std::vector<StageVertex> stageVertices;
		std::vector<int> stageIndices;

		buildStageMesh(stageVertices, stageIndices);

		stageMesh->updateStage(&stageVertices, &stageIndices);		
	}

	bool Stage::getDefaultStageTextures()
	{
		textures.push_back("RED");
		textures.push_back("BLUE");
		return true;
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

	bool Stage::createStageModel()
	{
		if (assets.modelStack.pushWorld(new Model(ID("MO_STAGE"), ID("ME_STAGE"), ID("MA_STAGE"))) != NULL)
		{
			return true;			
		}

		return false;
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

	int Stage::get(int x, int y, int z)
	{
		return stage[x + (y * STAGE_WIDTH) + (z * STAGE_WIDTH * STAGE_HEIGHT)];
	}

	void Stage::set(int x, int y, int z, int brush)
	{
		stage[x + (y * STAGE_WIDTH) + (z * STAGE_WIDTH * STAGE_HEIGHT)] = brush;
	}

	void Stage::drawBrush(std::vector<glm::ivec3>* slots, int brush)
	{
		for (auto it = slots->begin(); it != slots->end(); it++) 
		{
			set((*it).x, (*it).y, (*it).z, brush + 1);
		}
	}
}