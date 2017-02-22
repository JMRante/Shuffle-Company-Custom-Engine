/*

    SHUFFLE COMPANY SOURCE CODE

    sc_assets.h
    ------------------------------------



*/

#ifndef SC_ASSETS
#define SC_ASSETS

#include <stack>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

#include <GL/glew.h>
#include <SDL.h>

#include <glm/glm.hpp>

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
	};

	class Mesh
	{
	public:
		ID id;
		GLuint VAOid;
		GLuint VBOid;
		GLuint EBOid;

		int indexCount;

		Mesh(ID id);
		bool loadToGPU(std::string filepath);
		bool loadToGPU(std::vector<Vertex> *vertices, std::vector<int> *indices);
		bool loadToGPU(std::vector<StageVertex> *vertices, std::vector<int> *indices);
		void removeFromGPU();
	};

	class Texture
	{
	public:
		ID id;
		GLuint GLid;

		GLuint width;
		GLuint height;
		bool array;

		Texture(ID id);
		bool loadToGPU(std::string filepath);
		bool loadToGPU(GLuint width, GLuint height, GLuint* data);
		bool loadToGPU(GLuint width, GLuint height, std::vector<GLuint*> dataArray);
		void removeFromGPU();
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

		Sprite(ID id);
		bool loadToGPU(std::string filepath);
		void removeFromGPU();		
	};

	class Font
	{

	};

	class Shader
	{
	public:
		ID id;
		GLuint GLid;

		Shader(ID id);
		bool loadToGPU(std::string vertexShaderFilepath, std::string fragmentShaderFilepath);
		void removeFromGPU();
	};

	class Material
	{
	public:
		ID id;

		std::vector<int> integerMaterialArguments;
		std::vector<float> floatMaterialArguments;
		std::vector<glm::vec4> vec4MaterialArguments;
		std::vector<Texture*> textureMaterialArguments;

		Shader *shader;

		Material(ID id, std::vector<int> *ima, std::vector<float> *fma, std::vector<glm::vec4> *vma, std::vector<ID> *tma, ID shaderId);
	};

	class Model
	{
	public:
		ID id;

		Mesh *mesh;
		Material *material;
		std::vector<Model> subModels;

		glm::vec3 relativePosition;
		glm::vec3 relativeRotation;
		glm::vec3 relativeScale;

		Model(ID id, ID meshId, ID materialId);
		Model* addSubModel(ID id, ID meshId, ID materialId);
		Model* getSubModel(ID id);
	}; 

	class Assets
	{
	private:
		std::vector<Mesh> meshPool;
		std::vector<Texture> texturePool;
		std::vector<Sprite> spritePool;
		std::vector<Font> fontPool;
		std::vector<Shader> shaderPool;
		std::vector<Material> materialPool;
		std::vector<Model> modelPool;

		int worldMeshStart;
		int worldTextureStart;
		int worldSpriteStart;
		int worldFontStart;
		int worldShaderStart;
		int worldMaterialStart;
		int worldModelStart;

	public:
		bool loadMesh(ID id, std::string filepath);
		bool loadMesh(ID id, std::vector<Vertex> *vertices, std::vector<int> *indices);
		bool loadMesh(ID id, std::vector<StageVertex> *vertices, std::vector<int> *indices);

		bool loadTexture(ID id, std::string filepath);
		bool loadTexture(ID id, GLuint width, GLuint height, GLuint* data);
		bool loadTexture(ID id, GLuint width, GLuint height, std::vector<GLuint*> dataArray);

		bool loadSprite(ID id, std::string filepath);

		bool loadShader(ID id, std::string vertexShaderFilepath, std::string fragmentShaderFilepath);
		bool loadMaterial(ID id, std::vector<int> *ima, std::vector<float> *fma, std::vector<glm::vec4> *vma, std::vector<ID> *tma, ID shaderId);
		bool loadModel(ID id, ID meshId, ID materialId);

		void loadDefaults();

		Mesh* getMesh(ID id);
		Texture* getTexture(ID id);
		Sprite *getSprite(ID id);
		Shader* getShader(ID id);
		Material* getMaterial(ID id);
		Model* getModel(ID id);
	};

	extern Assets assets;
}

#endif