/*

    SHUFFLE COMPANY SOURCE CODE

    sc_assets.cpp
    ------------------------------------



*/

#include "sc_assets.h"

#include <IL/il.h>
#include <IL/ilu.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace sc
{
	Assets assets;

	/*
		Mesh
				*/
	Mesh::Mesh(ID id)
	{
		this->id = id;
		VAOid = 0;
		VBOid = 0;
		EBOid = 0;

		indexCount = 0;
	}

	bool Mesh::loadToGPU(std::string filepath)
	{
		std::vector<Vertex> currentVertices;
		std::vector<int> currentIndices;

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filepath.c_str());

		if (!err.empty()) 
		{
			std::cerr << err << std::endl;
		}

		if (!ret) 
		{
			return false;
		}
		
		//Iterate through faces of object
		size_t index_offset = 0;

		for (size_t f = 0; f < shapes[0].mesh.num_face_vertices.size(); f++) 
		{
			int fv = shapes[0].mesh.num_face_vertices[f];

			for (int v = 0; v < fv; v++) 
			{
				//Get vertex data
				Vertex tempVert;
				tinyobj::index_t idx = shapes[0].mesh.indices[index_offset + v];

				tempVert.position[0] = attrib.vertices[3 * idx.vertex_index + 0];
				tempVert.position[1] = attrib.vertices[3 * idx.vertex_index + 1];
				tempVert.position[2] = attrib.vertices[3 * idx.vertex_index + 2];

				tempVert.normal[0] = attrib.normals[3 * idx.normal_index + 0];
				tempVert.normal[1] = attrib.normals[3 * idx.normal_index + 1];
				tempVert.normal[2] = attrib.normals[3 * idx.normal_index + 2];

				tempVert.textureCoord[0] = attrib.texcoords[2 * idx.texcoord_index + 0];
				tempVert.textureCoord[1] = attrib.texcoords[2 * idx.texcoord_index + 1];

				//Check if vertex already exists; if not, add it.
				int index = -1;

				for (size_t e = 0; e < currentVertices.size(); e++)
				{
					if (tempVert.position[0] == currentVertices[e].position[0]
						&& tempVert.position[1] == currentVertices[e].position[1]
						&& tempVert.position[2] == currentVertices[e].position[2])
					{
						index = e;
						// LOG_D << "Repeat vertex " << e;
						break;
					}
				}

				if (index == -1)
				{
					currentVertices.push_back(tempVert);
					index = (int)currentVertices.size() - 1;				
				}

				currentIndices.push_back(index);
				// LOG_D << "Face: " << f << " Index: " << index << " VX: " << tempVert.position[0] << " VY: " << tempVert.position[1] << " VZ: " << tempVert.position[2];
			}

			index_offset += fv;
		}

		return loadToGPU(&currentVertices, &currentIndices);
	}

	bool Mesh::loadToGPU(std::vector<Vertex> *vertices, std::vector<int> *indices)
	{
		glGenVertexArrays(1, &VAOid);
		glGenBuffers(1, &VBOid);
		glGenBuffers(1, &EBOid);

		glBindVertexArray(VAOid);
			glBindBuffer(GL_ARRAY_BUFFER, VBOid);
			glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &((*vertices)[0]), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOid);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(GLuint), &((*indices)[0]), GL_STATIC_DRAW);

			//Position
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

			//Normal
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));

			//UV
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
		glBindVertexArray(0);

		indexCount = indices->size();

		GLenum error = glGetError();

		if (error != GL_NO_ERROR)
		{
			LOG_E << "Error loading mesh: " << gluErrorString(error);
			return false;
		}
		else
		{
			return true;
		}
	}

	bool Mesh::loadToGPU(std::vector<StageVertex> *vertices, std::vector<int> *indices)
	{
		glGenVertexArrays(1, &VAOid);
		glGenBuffers(1, &VBOid);
		glGenBuffers(1, &EBOid);

		glBindVertexArray(VAOid);
			glBindBuffer(GL_ARRAY_BUFFER, VBOid);
			glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(StageVertex), &((*vertices)[0]), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOid);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(GLuint), &((*indices)[0]), GL_STATIC_DRAW);

			//Position
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(StageVertex), (GLvoid*)0);

			//Normal
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(StageVertex), (GLvoid*)(3 * sizeof(GLfloat)));

			//UV
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(StageVertex), (GLvoid*)(6 * sizeof(GLfloat)));

			//Texture
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(StageVertex), (GLvoid*)(8 * sizeof(GLfloat)));
		glBindVertexArray(0);

		indexCount = indices->size();

		GLenum error = glGetError();

		if (error != GL_NO_ERROR)
		{
			LOG_E << "Error loading mesh: " << gluErrorString(error);
			return false;
		}
		else
		{
			return true;
		}
	}

	void Mesh::removeFromGPU()
	{
		if (VAOid != 0)
		{
			glDeleteVertexArrays(1, &VAOid);
			glDeleteBuffers(1, &VBOid);
			glDeleteBuffers(1, &EBOid);
			VAOid = 0;
			VBOid = 0;
			EBOid = 0;
		}
	}


	/*
		Texture
				*/
	Texture::Texture(ID id)
	{
		this->id = id;
		GLid = 0;
		array = false;
	}

	bool Texture::loadToGPU(std::string filepath)
	{
		bool success = false;

		//Load image with DevIL
		ILuint ilID = 0;
		ilGenImages(1, &ilID);
		ilBindImage(ilID);

		ILboolean ilSuccess = ilLoadImage(filepath.c_str());

		if (ilSuccess == IL_TRUE)
		{
			//Convert image to RGBA
			ilSuccess = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

			if (ilSuccess == IL_TRUE)
			{
				this->removeFromGPU();

				width = (GLuint)ilGetInteger(IL_IMAGE_WIDTH);
				height = (GLuint)ilGetInteger(IL_IMAGE_HEIGHT);

				//Pad non power of two textures for SPEED
				GLuint widthPT = powerOfTwo(width);
				GLuint heightPT = powerOfTwo(height);

				if (width != widthPT || height != heightPT)
				{
					iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);
					iluEnlargeCanvas((int)widthPT, (int)heightPT, 1);
				}

				//Generate texture ID
				glGenTextures(1, &GLid);

				//Load texture into OpenGL
				glBindTexture(GL_TEXTURE_2D, GLid);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthPT, heightPT, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLuint*)ilGetData());

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glGenerateMipmap(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);

				GLenum error = glGetError();

				if(error != GL_NO_ERROR)
				{
					LOG_E << "Error loading texture: " << gluErrorString(error);
					success = false;
				}
				else
				{
					success = true;
				}
			}

			//Delete file from memory
			ilDeleteImages(1, &ilID);
		}

		if(!success)
		{
			LOG_E << "Unable to load the texture " << filepath;
		}

		return success;
	}

	bool Texture::loadToGPU(GLuint width, GLuint height, GLuint* data)
	{
		this->removeFromGPU();

		//Generate texture ID
		glGenTextures(1, &GLid);

		//Load texture into OpenGL
		glBindTexture(GL_TEXTURE_2D, GLid);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		GLenum error = glGetError();

		if(error != GL_NO_ERROR)
		{
			LOG_E << "Error loading texture: " << gluErrorString(error);
			return false;
		}

		return true;
	}

	bool Texture::loadToGPU(GLuint width, GLuint height, std::vector<GLuint*> dataArray)
	{
		this->removeFromGPU();

		//Generate texture ID
		glGenTextures(1, &GLid);

		//Load texture into OpenGL
		glBindTexture(GL_TEXTURE_2D_ARRAY, GLid);
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, dataArray.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)0);

			for (size_t i = 0; i < dataArray.size(); i++)
			{
				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, (GLint)i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, dataArray[i]);
			}

			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	        //glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		glBindTexture(GL_TEXTURE_2D, 0);

		GLenum error = glGetError();

		if(error != GL_NO_ERROR)
		{
			LOG_E << "Error loading texture: " << gluErrorString(error);
			return false;
		}

		array = true;
		return true;
	}

	void Texture::removeFromGPU()
	{
		if (GLid != 0)
		{
			glDeleteTextures(1, &GLid);
			GLid = 0;
		}
	}


	/*
		Shader
				*/
	Shader::Shader(ID id)
	{
		this->id = id;
		GLid = 0;
	}

	bool Shader::loadToGPU(std::string vertexShaderFilepath, std::string fragmentShaderFilepath)
	{
		//Has shader already been built?
		if (GLid != 0)
		{
			LOG_E << "Shader already built";		
			return false;		
		}

		//Load Shader
		GLint success;
		GLchar infoLog[512];

		//Load shader files
		const GLchar* vertexShaderSource = sc::fileRead(vertexShaderFilepath.c_str());

		if (vertexShaderSource == NULL)
		{
			LOG_E << "Failed to open vertex shader";		
			return false;
		}

		const GLchar* fragmentShaderSource = sc::fileRead(fragmentShaderFilepath.c_str());

		if (fragmentShaderSource == NULL)
		{
			LOG_E << "Failed to open fragment shader";		
			return false;
		}

		//Compile Vertex Shader
		GLuint vertexShader;

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			LOG_E << "Vertex Shader Compile Error: " << infoLog;
			return false;
		}

		//Compile Fragment Shader
		GLuint fragmentShader;

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			LOG_E << "Fragment Shader Compile Error: " << infoLog;
			return false;
		}

		//Link shaders into program
		GLid = glCreateProgram();

		glAttachShader(GLid, vertexShader);
		glAttachShader(GLid, fragmentShader);
		glLinkProgram(GLid);

		glGetProgramiv(GLid, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(GLid, 512, NULL, infoLog);
			LOG_E << "Shader Program Linking Error: " << infoLog;
			return false;		
		}

		//Clean-up Shader Code from memory
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return true;
	}

	void Shader::removeFromGPU()
	{
		if (GLid != 0)
		{
			glDeleteProgram(GLid);
			GLid = 0;
		}
	}


	/*
		Material
				*/
	Material::Material(ID id, std::vector<int> *ima, std::vector<float> *fma, std::vector<glm::vec4> *vma, std::vector<ID> *tma, ID shaderId)
	{
		this->id = id;

		if (ima != NULL)
		{
			for (size_t i = 0; i < ima->size(); i++)
			{
				integerMaterialArguments.push_back((*ima)[i]);
			}
		}

		if (fma != NULL)
		{
			for (size_t i = 0; i < fma->size(); i++)
			{
				floatMaterialArguments.push_back((*fma)[i]);
			}
		}

		if (vma != NULL)
		{
			for (size_t i = 0; i < vma->size(); i++)
			{
				vec4MaterialArguments.push_back((*vma)[i]);
			}
		}

		if (tma != NULL)
		{
			for (size_t i = 0; i < tma->size(); i++)
			{
				textureMaterialArguments.push_back(assets.getTexture((*tma)[i]));
			}
		}

		shader = assets.getShader(shaderId);
	}


	/*
		Model
				*/
	Model::Model(ID id, ID meshId, ID materialId)
	{
		this->id = id;

		mesh = assets.getMesh(meshId);
		material = assets.getMaterial(materialId);

		relativePosition = glm::vec3(0.0f, 0.0f, 0.0f);
		relativeRotation = glm::vec3(0.0f, 0.0f, 0.0f);
		relativeScale = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	Model* Model::addSubModel(ID id, ID meshId, ID materialId)
	{
		subModels.push_back(Model(id, meshId, materialId));
		return &subModels.back();
	}

	Model* Model::getSubModel(ID id)
	{
		for (size_t i = 0; i < subModels.size(); i++)
		{
			if (subModels[i].id.is(id))
			{
				return &subModels[i];
			}
		}

		//Eventually should return a default mesh object preloaded.
		return NULL;		
	}


	/*
		Assets
				*/
	bool Assets::loadMesh(ID id, std::string filepath)
	{
		meshPool.push_back(Mesh(id));
		return meshPool.back().loadToGPU(filepath);
	}

	bool Assets::loadMesh(ID id, std::vector<Vertex> *vertices, std::vector<int> *indices)
	{
		meshPool.push_back(Mesh(id));
		return meshPool.back().loadToGPU(vertices, indices);
	}

	bool Assets::loadMesh(ID id, std::vector<StageVertex> *vertices, std::vector<int> *indices)
	{
		meshPool.push_back(Mesh(id));
		return meshPool.back().loadToGPU(vertices, indices);
	}


	bool Assets::loadTexture(ID id, std::string filepath)
	{
		texturePool.push_back(Texture(id));
		return texturePool.back().loadToGPU(filepath);
	}

	bool Assets::loadTexture(ID id, GLuint width, GLuint height, GLuint* data)
	{
		texturePool.push_back(Texture(id));
		return texturePool.back().loadToGPU(width, height, data);
	}

	bool Assets::loadTexture(ID id, GLuint width, GLuint height, std::vector<GLuint*> dataArray)
	{
		texturePool.push_back(Texture(id));
		return texturePool.back().loadToGPU(width, height, dataArray);
	}


	bool Assets::loadShader(ID id, std::string vertexShaderFilepath, std::string fragmentShaderFilepath)
	{
		shaderPool.push_back(Shader(id));
		return shaderPool.back().loadToGPU(vertexShaderFilepath, fragmentShaderFilepath);
	}

	bool Assets::loadMaterial(ID id, std::vector<int> *ima, std::vector<float> *fma, std::vector<glm::vec4> *vma, std::vector<ID> *tma, ID shaderId)
	{
		materialPool.push_back(Material(id, ima, fma, vma, tma, shaderId));
		return true;
	}

	bool Assets::loadModel(ID id, ID meshId, ID materialId)
	{
		modelPool.push_back(Model(id, meshId, materialId));
		return true;
	}

	void Assets::loadDefaults()
	{
		//Load default assets
		//Quad
		Vertex tempVert;
		std::vector<Vertex> vecVert;

		tempVert.position = glm::vec3(0.0f, 0.0f, 0.0f);
		tempVert.normal = glm::vec3(0.0f, 0.0f, 1.0f);
		tempVert.textureCoord = glm::vec2(0.0f, 0.0f);
		vecVert.push_back(tempVert);

		tempVert.position = glm::vec3(1.0f, 0.0f, 0.0f);
		tempVert.textureCoord = glm::vec2(1.0f, 0.0f);
		vecVert.push_back(tempVert);

		tempVert.position = glm::vec3(0.0f, 1.0f, 0.0f);
		tempVert.textureCoord = glm::vec2(0.0f, 1.0f);
		vecVert.push_back(tempVert);

		tempVert.position = glm::vec3(1.0f, 1.0f, 0.0f);
		tempVert.textureCoord = glm::vec2(1.0f, 1.0f);
		vecVert.push_back(tempVert);

		static const int ind[] = {0, 1, 3, 0, 3, 2};
		std::vector<int> vecInd(ind, ind + sizeof(ind) / sizeof(ind[0]));

		loadMesh(ID("ME_QUAD"), &vecVert, &vecInd);
		loadMesh(ID("ME_SPHERE"), "Resources/Meshes/ME_SPHERE.obj");

		//Load Textures
		loadTexture(ID("TX_SPR1"), "Resources/Textures/testSprite.png");

		//Load Shaders
		loadShader(ID("SH_PASS"), "Resources/Shaders/sc_shader_testVertex.glsl", "Resources/Shaders/sc_shader_testFragment.glsl");
		loadShader(ID("SH_TEX"), "Resources/Shaders/sc_shader_testTextureVertex.glsl", "Resources/Shaders/sc_shader_testTextureFragment.glsl");
		loadShader(ID("SH_STAGE"), "Resources/Shaders/sc_shader_stageVertex.glsl", "Resources/Shaders/sc_shader_stageFragment.glsl");
		loadShader(ID("SH_COLOR"), "Resources/Shaders/sc_shader_flatColorVertex.glsl", "Resources/Shaders/sc_shader_flatColorFragment.glsl");
		loadShader(ID("SH_SPRITE"), "Resources/Shaders/sc_shader_spriteVertex.glsl", "Resources/Shaders/sc_shader_spriteFragment.glsl");

		std::vector<glm::vec4> tempVec4;
		tempVec4.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		loadMaterial(ID("MA_RED"), NULL, NULL, &tempVec4, NULL, ID("SH_PASS"));
		tempVec4.clear();
		tempVec4.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		loadMaterial(ID("MA_BLUE"), NULL, NULL, &tempVec4, NULL, ID("SH_PASS"));

		loadModel(ID("MO_TESTA"), ID("ME_QUAD"), ID("MA_RED"));
		loadModel(ID("MO_TESTB"), ID("ME_SPHERE"), ID("MA_BLUE"));
	}

	// Someday template these get functions
	Mesh* Assets::getMesh(ID id)
	{
		for (auto ai = meshPool.begin(); ai != meshPool.end(); ai++)
		{
			if (ai->id.is(id))
			{
				return &(*ai);
			}
		}

		//Eventually should return a default mesh object preloaded.
		LOG_E << "Failed to get mesh resource " << id.get();

		return NULL;
	}

	Texture* Assets::getTexture(ID id)
	{
		for (auto ai = texturePool.begin(); ai != texturePool.end(); ai++)
		{
			if (ai->id.is(id))
			{
				return &(*ai);
			}
		}

		//Eventually should return a default mesh object preloaded.
		LOG_E << "Failed to get texture resource " << id.get();

		return NULL;
	}

	Shader* Assets::getShader(ID id)
	{
		for (auto ai = shaderPool.begin(); ai != shaderPool.end(); ai++)
		{
			if (ai->id.is(id))
			{
				return &(*ai);
			}
		}

		//Eventually should return a default mesh object preloaded.
		LOG_E << "Failed to get shader resource " << id.get();

		return NULL;
	}

	Material* Assets::getMaterial(ID id)
	{
		for (auto ai = materialPool.begin(); ai != materialPool.end(); ai++)
		{
			if (ai->id.is(id))
			{
				return &(*ai);
			}
		}

		//Eventually should return a default mesh object preloaded.
		LOG_E << "Failed to get material resource " << id.get();

		return NULL;
	}

	Model* Assets::getModel(ID id)
	{
		for (auto ai = modelPool.begin(); ai != modelPool.end(); ai++)
		{
			if (ai->id.is(id))
			{
				return &(*ai);
			}
		}

		//Eventually should return a default mesh object preloaded.
		LOG_E << "Failed to get model resource " << id.get();

		return NULL;
	}
}