/*

    SHUFFLE COMPANY SOURCE CODE

    sc_assets.h
    ------------------------------------



*/

#ifndef SC_ASSETS
#define SC_ASSETS

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <cmath>

#include <GL/glew.h>
#include <SDL.h>

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "sc_log.h"
#include "sc_utility.h"

namespace sc
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoord;
	};

	struct StageVertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoord;
		float textureNum;
		int id;
		glm::vec3 idColor;
	};

	class Mesh
	{
	public:
		ID id;
		GLuint VAOid;
		GLuint VBOid;
		GLuint EBOid;

		int indexCount;

		Mesh(ID id, std::string filepath);
		Mesh(ID id, std::vector<Vertex> *vertices, std::vector<int> *indices);
		Mesh(ID id, std::vector<StageVertex> *vertices, std::vector<int> *indices);
		~Mesh();

		void updateStage(std::vector<StageVertex> *vertices, std::vector<int> *indices);
	};

	class Texture
	{
	public:
		ID id;
		GLuint GLid;

		GLuint width;
		GLuint height;
		bool array;

		Texture(ID id, std::string filepath);
		Texture(ID id, GLuint width, GLuint height, GLuint* data);
		Texture(ID id, GLuint width, GLuint height, std::vector<GLuint*> dataArray);
		~Texture();
	};

	class Sprite
	{
	public:
		ID id;
		GLuint GLid;

		GLuint width;
		GLuint height;
		float texCoordX;
		float texCoordY;

		Sprite(ID id, std::string filepath);
		~Sprite();
	};

	struct FontCharacter
	{
		glm::vec4 textureCoords;
		glm::ivec2 size;
		glm::ivec2 bearing;
		GLuint advance;
		
		GLubyte* bitmap;
	};

	class Font
	{
	public:
		ID id;
		FT_Face face;
		std::map<GLchar, FontCharacter> characters;

		GLuint textureGLid;
		GLuint textureWidth;
		GLuint textureHeight;

		int maxCharWidth;
		int maxCharHeight;

		static GLuint VAOid;
		static GLuint VBOid;

		Font(ID id, std::string filepath, int height);
		~Font();

		static void loadFontQuadToGPU();
		static void clearFontQuadFromGPU();
	};

	class Shader
	{
	public:
		ID id;
		GLuint GLid;

		Shader(ID id, std::string vertexShaderFilepath, std::string fragmentShaderFilepath);
		~Shader();
	};

	class Material
	{
	public:
		ID id;

		std::vector<int> integerMaterialArguments;
		std::vector<float> floatMaterialArguments;
		std::vector<glm::vec4> vec4MaterialArguments;
		std::vector<Texture*> textureMaterialArguments;

		Shader* shader;

		Material(ID id, std::vector<int> *ima, std::vector<float> *fma, std::vector<glm::vec4> *vma, std::vector<ID> *tma, ID shaderId);
		~Material();
	};

	class Model
	{
	public:
		ID id;

		Mesh* mesh;
		Material* material;
		std::vector<Model*> subModels;

		glm::vec3 relativePosition;
		glm::vec3 relativeRotation;
		glm::vec3 relativeScale;

		Model(ID id, ID meshId, ID materialId);
		~Model();
		Model* addSubModel(Model* model);
		Model* getSubModel(ID id);
	}; 

	template <class T>
	class AssetStack
	{
	private:
		std::vector<T*> stack;
		size_t baseAssetCount;
		bool baseLocked;

	public:
		AssetStack()
		{
			baseAssetCount = 0;
			baseLocked = false;
		}

		bool areWorldAssetsLoaded()
		{
			if (stack.size() > baseAssetCount)
			{
				return true;
			}

			return false;
		}

		T* pushBase(T* t)
		{
			if (!baseLocked)
			{
				if (t->id.is(ID("ERROR")))
				{
					LOG_E << "Tried to load broken asset";
					return NULL;
				}

				for (auto ai = stack.begin(); ai != stack.end(); ai++)
				{
					if ((*ai)->id.is(t->id))
					{
						LOG_E << "There already exists a " << t->id.get();
						return NULL;
					}
				}

				stack.push_back(t);
				return stack.back();
			}

			LOG_E << "Cannot currently load asset to base, load to world if needed";
			return NULL;
		}

		T* pushWorld(T* t)
		{
			if (t->id.is(ID("ERROR")))
			{
				LOG_E << "Tried to load broken asset";
				return NULL;
			}

			if (!baseLocked)
			{
				baseLocked = true;
				baseAssetCount = stack.size();
			}

			for (auto ai = stack.begin(); ai != stack.end(); ai++)
			{
				if ((*ai)->id.is(t->id))
				{
					LOG_E << "There already exists a " << t->id.get();
					return NULL;
				}
			}

			stack.push_back(t);
			return stack.back();
		}

		void clearBase()
		{
			while (!stack.empty())
			{
				delete stack.back(); 
				stack.pop_back();
			}
		}

		void clearWorld()
		{
			while (stack.size() > baseAssetCount)
			{
				delete stack.back(); 
				stack.pop_back();
			}

			baseLocked = false;
		}

		T* get(ID id)
		{
			for (auto ai = stack.begin(); ai != stack.end(); ai++)
			{
				if ((*ai)->id.is(id))
				{
					return *ai;
				}
			}

			return NULL;
		}

		typename std::vector<T>::iterator begin()
		{
			return stack.begin();
		}

		typename std::vector<T>::iterator end()
		{
			return stack.end();
		}
	};


	class Assets
	{
	public:
		AssetStack<Mesh> meshStack;
		AssetStack<Texture> textureStack;
		AssetStack<Sprite> spriteStack;
		AssetStack<Font> fontStack;
		AssetStack<Shader> shaderStack;
		AssetStack<Material> materialStack;
		AssetStack<Model> modelStack;

		FT_Library fontLibrary;

		Assets();

		bool areWorldAssetsLoaded();

		void loadBaseAssets();
		void clearBaseAssets();
		void clearWorldAssets();
	};

	extern Assets assets;
}

#endif