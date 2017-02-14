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
	Brush::Brush(unsigned char texNum)
	{
		tex_E = texNum;
		tex_N = texNum;
		tex_W = texNum;
		tex_S = texNum;
		tex_T = texNum;
		tex_B = texNum;		
	}

	bool Stage::loadStage(std::string filepath)
	{
		if (!readStageFile(filepath))
		{
			return false;
		}

		if (!loadStageTextures())
		{
			return false;
		}

		if (!buildStageMesh())
		{
			return false;
		}

		if (!createStageModel())
		{
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
			sc::Tokenizer t(&file);
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
							unsigned char textureNum = getTextureNum(t.getToken());

							if (textureNum == 255)
							{
								if (textures.size() >= MAX_SIMPLE_TEXTURES)
								{
									LOG_E << "Stage has too many simple textures";
									return false;
								}

								textures.push_back(t.getToken());
								textureNum = (unsigned char)(textures.size() - 1);
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

							unsigned char brush;
							int x = 0;
							int y = 0;
							int z = 0;

							t.next();
							if (t.checkType("int"))
							{
								while (t.checkType("int"))
								{
									brush = (unsigned char)(atoi(t.getToken().c_str()));
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
			LOG_E << "Could not open shuff file " << filepath;
			return false;
		}

		file.close();
		LOG_E << "Invalid shuff file data";
		return false;
	}

	bool Stage::loadStageTextures()
	{
		ILuint baseId = 0;
		ilGenImages(1, &baseId);
		ilBindImage(baseId);

		ILboolean success = ilTexImage(STAGE_TEXTURE_DIM, STAGE_TEXTURE_DIM, 0, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL);

		if (success == IL_TRUE)
		{
			for (size_t i = 0; i < textures.size(); i++)
			{
				ILuint tempId = 0;
				ilGenImages(1, &tempId);
				ilBindImage(tempId);

				success = ilLoadImage(("Resources/StageTextures/Simple/Albedo/ST_" + textures[i] + ".png").c_str());

				if (success == IL_TRUE)
				{
					success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

					if (success == IL_TRUE)
					{
						if (ilGetInteger(IL_IMAGE_WIDTH) == SIMPLE_TEXTURE_DIM && ilGetInteger(IL_IMAGE_HEIGHT) == SIMPLE_TEXTURE_DIM)
						{
							ilBindImage(baseId);
							success = ilOverlayImage(tempId, (ILint)getTextureX(i), (ILint)getTextureY(i), 0);

							if (success == IL_TRUE)
							{
								ilDeleteImages(1, &tempId);
							}
							else
							{
								LOG_E << "Failed to copy simple stage texture to stage texture";
								return false;
							}
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
		}
		else
		{
			LOG_E << "Error creating stage texture";
			return false;
		}

		assets.loadTexture("TX_STAGE", STAGE_TEXTURE_DIM, STAGE_TEXTURE_DIM, (GLuint*)ilGetData());
		ilDeleteImages(1, &baseId);

		std::vector<std::string> tempString;
		tempString.push_back("TX_STAGE");
		assets.loadMaterial("MA_STAGE", NULL, NULL, NULL, &tempString, "SH_TEX");

		return true;
	}

	bool Stage::buildStageMesh()
	{
		return true;
	}

	bool Stage::createStageModel()
	{
		assets.loadModel("MO_STAGE", "ME_FLAT", "MA_STAGE");
		return true;
	}

	int Stage::getTextureX(unsigned char textureNum)
	{
		int texNum = (int)textureNum;

		if (texNum < 128)
		{
			return (texNum * SIMPLE_TEXTURE_DIM) % STAGE_TEXTURE_DIM;
		}

		if (texNum < 192)
		{
			return (texNum * SIMPLE_TEXTURE_DIM) % (STAGE_TEXTURE_DIM / 2);
		}

		if (texNum == 192)
		{
			return 2186;
		}

		if (texNum == 193)
		{
			return 3072;
		}

		return -1;
	}

	int Stage::getTextureY(unsigned char textureNum)
	{
		int texNum = (int)textureNum;

		if (texNum < 192)
		{
			return ((texNum * SIMPLE_TEXTURE_DIM) / STAGE_TEXTURE_DIM) * SIMPLE_TEXTURE_DIM;
		}

		if (texNum == 192 || texNum == 193)
		{
			return 2048;
		}

		return -1;
	}

	unsigned char Stage::getTextureNum(std::string textureName)
	{
		size_t i = 0;

		while (i < textures.size())
		{
			if (textures[i].compare(textureName) == 0)
			{
				return (unsigned char)i;
			}

			i++;
		}

		return 255;
	}

	sc::Model* Stage::getModel()
	{
		return model;
	}
}