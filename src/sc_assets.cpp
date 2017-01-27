/*

    SHUFFLE COMPANY SOURCE CODE

    sc_assets.cpp
    ------------------------------------



*/

#include "sc_assets.h"
#include <IL/il.h>

namespace sc
{
	/*
		Mesh
				*/
	Mesh::Mesh(ID id)
	{
		this.id = id;
		VBO = 0;
		VAO = 0;
		EBO = 0;
	}

	bool Mesh::loadFromFile(std::string filepath)
	{

	}

	bool loadFromData(std::vector<Vertex> vertices, std::vector<int> indices)
	{

	}

	void Mesh::setToRender()
	{

	}

	void Mesh::remove()
	{

	}


	/*
		Texture
				*/
	Texture::Texture(ID id)
	{
		this.id = id;
		GLid = 0;
	}

	bool Texture::loadFromFile(std::string filepath)
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
			    this.remove();

			    width = (GLuint)ilGetInteger(IL_IMAGE_WIDTH);
			    height = (GLuint)ilGetInteger(IL_IMAGE_HEIGHT);

			    //Generate texture ID
			    glGenTextures(1, GLid);

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
			        success false;
			    }
			    else
			    {
					success true;
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

	void Texture::setToRender()
	{
        glBindTexture(GL_TEXTURE_2D, GLid);
	}

	void Texture::remove()
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
		this.id = id;
		GLid = 0;
	}

	bool Shader::buildFromFiles(std::string vertexShaderFilepath, std::string fragmentShaderFilepath)
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

	void Shader::setToRender()
	{
		glUseProgram(GLid);
	}

	void Shader::remove()
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
	Material::Material(ID id, Texture *textureA, Shader *shader)
	{
		this.id = id;
	}

	Material::Material(ID id, Texture *textureA, Texture *textureB, Shader *shader)
	{
		this.id = id;
	}

	Material::Material(ID id, Texture *textureA, Texture *textureB, Texture *textureC, Shader *shader)
	{
		this.id = id;
	}

	Material::Material(ID id, Texture *textureA, Texture *textureB, Texture *textureC, Texture *textureD, Shader *shader)
	{
		this.id = id;
	}

	void Material::setToRender()
	{

	}

	void Material::remove()
	{

	}


	/*
		Assets
				*/
	void Assets::loadBaseAssets()
	{

	}


	bool Assets::loadMesh(std::string filepath)
	{

	}

	bool Assets::loadMesh(Mesh *mesh)
	{

	}


	bool Assets::loadTexture(std::string filepath)
	{

	}

	bool Assets::loadTexture(Texture *texture)
	{

	}


	bool Assets::loadMaterial(Material *material)
	{

	}


	bool Assets::loadWorldMesh()
	{

	}

	bool Assets::loadWorldTexture()
	{

	}
}