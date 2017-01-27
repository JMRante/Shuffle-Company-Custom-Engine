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

#include <GL/glew.h>
#include <SDL.h>

#include <glm/glm.hpp>

#include "sc_log.h"
#include "sc_utilities.h"

namespace sc
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};

	class Mesh
	{
	private:
		std::vector<Vertex> vertices;
		std::vector<int> indices;

	public:
		ID id;
		GLuint VBO;
		GLuint VAO;
		GLuint EBO;

		Mesh(ID id);
		bool loadFromFile(std::string filepath);
		bool loadFromData(std::vector<Vertex> vertices, std::vector<int> indices);
		void setToRender();
		void remove();
	};

	class Texture
	{
	public:
		ID id;
		GLuint GLid;

		GLuint width;
		GLuint height;

		Texture(ID id);
		bool loadFromFile(std::string filepath);
		void setToRender();
		void remove();
	};

	class Shader
	{
	public:
		ID id;
		GLuint GLid;

		Shader(ID id);
		bool buildFromFiles(std::string vertexShaderFilepath, std::string fragmentShaderFilepath);
		void setToRender();
		void remove();
	};

	class Material
	{
	private:
		Shader shader;
		Texture textureA;
		Texture textureB;
		Texture textureC;
		Texture textureD;

	public:
		ID id;

		Material(ID id, Texture *textureA, Shader *shader);
		Material(ID id, Texture *textureA, Texture *textureB, Shader *shader);
		Material(ID id, Texture *textureA, Texture *textureB, Texture *textureC, Shader *shader);
		Material(ID id, Texture *textureA, Texture *textureB, Texture *textureC, Texture *textureD, Shader *shader);
		void setToRender();
		void remove();
	};

	class Assets
	{
	private:
		std::stack<Mesh> meshStack;
		std::stack<Texture> textureStack;
		std::stack<Shader> shaderStack;
		std::stack<Material> materialStack;

		int worldMeshStackStart;
		int worldTextureStackStart;
		int worldShaderStack;
		int worldMaterialStackStart;

	public:
		void loadBaseAssets();

		bool loadMesh(std::string filepath);
		bool loadMesh(Mesh *mesh);

		bool loadTexture(std::string filepath);
		bool loadTexture(Texture *texture);

		bool loadMaterial(Material *material);

		bool loadWorldMesh();
		bool loadWorldTexture();
	};
}

#endif