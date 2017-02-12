/*

    SHUFFLE COMPANY SOURCE CODE

    sc_assets.cpp
    ------------------------------------



*/

#include "sc_assets.h"
#include <IL/il.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace sc
{
	Assets assets;

	/*
		Mesh
				*/
	Mesh::Mesh(std::string id)
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
	Texture::Texture(std::string id)
	{
		this->id = id;
		GLid = 0;
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

				//Generate texture ID
				glGenTextures(1, &GLid);

				//Load texture into OpenGL
				glBindTexture(GL_TEXTURE_2D, GLid);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLuint*)ilGetData());

					glGenerateMipmap(GL_TEXTURE_2D);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
	Shader::Shader(std::string id)
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
	Material::Material(std::string id, std::vector<int> *ima, std::vector<float> *fma, std::vector<glm::vec4> *vma, std::vector<std::string> *tma, std::string shaderId)
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
	Model::Model(std::string id, std::string meshId, std::string materialId)
	{
		this->id = id;

		mesh = assets.getMesh(meshId);
		material = assets.getMaterial(materialId);

		relativePosition = glm::vec3(0.0f, 0.0f, 0.0f);
		relativeRotation = glm::vec3(0.0f, 0.0f, 0.0f);
		relativeScale = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	Model* Model::addSubModel(std::string id, std::string meshId, std::string materialId)
	{
		subModels.push_back(Model(id, meshId, materialId));
		return &subModels.back();
	}

	Model* Model::getSubModel(std::string id)
	{
		for (size_t i = 0; i < subModels.size(); i++)
		{
			if (subModels[i].id == id)
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
	bool Assets::loadMesh(std::string id, std::string filepath)
	{
		meshPool.push_back(Mesh(id));
		return meshPool.back().loadToGPU(filepath);
	}

	bool Assets::loadMesh(std::string id, std::vector<Vertex> *vertices, std::vector<int> *indices)
	{
		meshPool.push_back(Mesh(id));
		return meshPool.back().loadToGPU(vertices, indices);
	}

	bool Assets::loadTexture(std::string id, std::string filepath)
	{
		texturePool.push_back(Texture(id));
		return texturePool.back().loadToGPU(filepath);
	}

	bool Assets::loadShader(std::string id, std::string vertexShaderFilepath, std::string fragmentShaderFilepath)
	{
		shaderPool.push_back(Shader(id));
		return shaderPool.back().loadToGPU(vertexShaderFilepath, fragmentShaderFilepath);
	}

	bool Assets::loadMaterial(std::string id, std::vector<int> *ima, std::vector<float> *fma, std::vector<glm::vec4> *vma, std::vector<std::string> *tma, std::string shaderId)
	{
		materialPool.push_back(Material(id, ima, fma, vma, tma, shaderId));
		return true;
	}

	bool Assets::loadModel(std::string id, std::string meshId, std::string materialId)
	{
		modelPool.push_back(Model(id, meshId, materialId));
		return true;
	}

	Mesh* Assets::getMesh(std::string id)
	{
		for (size_t i = 0; i < meshPool.size(); i++)
		{
			if (meshPool[i].id.compare(id) == 0)
			{
				return &meshPool[i];
			}
		}

		//Eventually should return a default mesh object preloaded.
		LOG_E << "Failed to get mesh resource " << id;

		return NULL;
	}

	Texture* Assets::getTexture(std::string id)
	{
		for (size_t i = 0; i < texturePool.size(); i++)
		{
			if (texturePool[i].id.compare(id) == 0)
			{
				return &texturePool[i];
			}
		}

		//Eventually should return a default mesh object preloaded.
		LOG_E << "Failed to get texture resource " << id;

		return NULL;
	}

	Shader* Assets::getShader(std::string id)
	{
		for (size_t i = 0; i < shaderPool.size(); i++)
		{
			if (shaderPool[i].id.compare(id) == 0)
			{
				return &shaderPool[i];
			}
		}

		//Eventually should return a default mesh object preloaded.
		LOG_E << "Failed to get shader resource " << id;

		return NULL;
	}

	Material* Assets::getMaterial(std::string id)
	{
		for (size_t i = 0; i < materialPool.size(); i++)
		{
			if (materialPool[i].id.compare(id) == 0)
			{
				return &materialPool[i];
			}
		}

		//Eventually should return a default mesh object preloaded.
		LOG_E << "Failed to get material resource " << id;

		return NULL;
	}

	Model* Assets::getModel(std::string id)
	{
		for (size_t i = 0; i < modelPool.size(); i++)
		{
			if (modelPool[i].id.compare(id) == 0)
			{
				return &modelPool[i];
			}
		}

		//Eventually should return a default mesh object preloaded.
		LOG_E << "Failed to get model resource " << id;

		return NULL;
	}
}