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

	class Mesh
	{
	public:
		std::string id;
		GLuint VAOid;
		GLuint VBOid;
		GLuint EBOid;

		int indexCount;

		Mesh(std::string id);
		bool loadToGPU(std::string filepath);
		bool loadToGPU(std::vector<Vertex> *vertices, std::vector<int> *indices);
		void removeFromGPU();
	};

	class Texture
	{
	public:
		std::string id;
		GLuint GLid;

		GLuint width;
		GLuint height;

		Texture(std::string id);
		bool loadToGPU(GLuint width, GLuint height, GLuint* data);
		bool loadToGPU(std::string filepath);
		void removeFromGPU();
	};

	class Shader
	{
	public:
		std::string id;
		GLuint GLid;

		Shader(std::string id);
		bool loadToGPU(std::string vertexShaderFilepath, std::string fragmentShaderFilepath);
		void removeFromGPU();
	};

	class Material
	{
	public:
		std::string id;

		std::vector<int> integerMaterialArguments;
		std::vector<float> floatMaterialArguments;
		std::vector<glm::vec4> vec4MaterialArguments;
		std::vector<Texture*> textureMaterialArguments;

		Shader *shader;

		Material(std::string id, std::vector<int> *ima, std::vector<float> *fma, std::vector<glm::vec4> *vma, std::vector<std::string> *tma, std::string shaderId);
	};

	class Model
	{
	public:
		std::string id;

		Mesh *mesh;
		Material *material;
		std::vector<Model> subModels;

		glm::vec3 relativePosition;
		glm::vec3 relativeRotation;
		glm::vec3 relativeScale;

		Model(std::string id, std::string meshId, std::string materialId);
		Model* addSubModel(std::string id, std::string meshId, std::string materialId);
		Model* getSubModel(std::string id);
	}; 

	class Assets
	{
	private:
		std::vector<Mesh> meshPool;
		std::vector<Texture> texturePool;
		std::vector<Shader> shaderPool;
		std::vector<Material> materialPool;
		std::vector<Model> modelPool;

		int worldMeshStart;
		int worldTextureStart;
		int worldShaderStart;
		int worldMaterialStart;
		int worldModelStart;

	public:
		bool loadMesh(std::string id, std::string filepath);
		bool loadMesh(std::string id, std::vector<Vertex> *vertices, std::vector<int> *indices);
		bool loadTexture(std::string id, std::string filepath);
		bool loadTexture(std::string id, GLuint width, GLuint height, GLuint* data);
		bool loadShader(std::string id, std::string vertexShaderFilepath, std::string fragmentShaderFilepath);
		bool loadMaterial(std::string id, std::vector<int> *ima, std::vector<float> *fma, std::vector<glm::vec4> *vma, std::vector<std::string> *tma, std::string shaderId);
		bool loadModel(std::string id, std::string meshId, std::string materialId);

		void loadDefaults();

		Mesh* getMesh(std::string id);
		Texture* getTexture(std::string id);
		Shader* getShader(std::string id);
		Material* getMaterial(std::string id);
		Model* getModel(std::string id);
	};

	extern Assets assets;
}

#endif