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
	Mesh::Mesh(ID id, std::string filepath)
	{
		this->id = id;
		VAOid = 0;
		VBOid = 0;
		EBOid = 0;

		indexCount = 0;

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
			LOG_E << "There was an error loading the mesh file " << filepath;
			this->id = CTID("ERROR");
			return;
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

		glGenVertexArrays(1, &VAOid);
		glGenBuffers(1, &VBOid);
		glGenBuffers(1, &EBOid);

		glBindVertexArray(VAOid);
			glBindBuffer(GL_ARRAY_BUFFER, VBOid);
			glBufferData(GL_ARRAY_BUFFER, currentVertices.size() * sizeof(Vertex), &(currentVertices[0]), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOid);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentIndices.size() * sizeof(GLuint), &(currentIndices[0]), GL_STATIC_DRAW);

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

		indexCount = currentIndices.size();

		GLenum error = glGetError();

		if (error != GL_NO_ERROR)
		{
			LOG_E << "Error loading mesh: " << gluErrorString(error);
			this->id = CTID("ERROR");
		}
	}

	Mesh::Mesh(ID id, std::vector<Vertex> *vertices, std::vector<int> *indices)
	{
		this->id = id;
		VAOid = 0;
		VBOid = 0;
		EBOid = 0;

		indexCount = 0;

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
			this->id = CTID("ERROR");
		}
	}

	Mesh::Mesh(ID id, std::vector<StageVertex> *vertices, std::vector<int> *indices)
	{
		this->id = id;
		VAOid = 0;
		VBOid = 0;
		EBOid = 0;

		indexCount = 0;

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
			this->id = CTID("ERROR");
		}
	}

	Mesh::~Mesh()
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

	void Mesh::updateStage(std::vector<StageVertex> *vertices, std::vector<int> *indices)
	{
		indexCount = 0;

		glBindBuffer(GL_ARRAY_BUFFER, VBOid);
			glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(StageVertex), &((*vertices)[0]), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOid);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(GLuint), &((*indices)[0]), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		indexCount = indices->size();

		GLenum error = glGetError();

		if (error != GL_NO_ERROR)
		{
			LOG_E << "Error updating mesh: " << gluErrorString(error);
		}
	}


	/*
		Texture
				*/
	Texture::Texture(ID id, std::string filepath)
	{
		this->id = id;
		GLid = 0;
		array = false;

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
				width = (GLuint)ilGetInteger(IL_IMAGE_WIDTH);
				height = (GLuint)ilGetInteger(IL_IMAGE_HEIGHT);

				//Generate texture ID
				glGenTextures(1, &GLid);

				//Load texture into OpenGL
				glBindTexture(GL_TEXTURE_2D, GLid);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLuint*)ilGetData());

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
			this->id = CTID("ERROR");
		}
	}

	Texture::Texture(ID id, GLuint width, GLuint height, GLuint* data)
	{
		this->id = id;
		GLid = 0;
		array = false;

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
			this->id = CTID("ERROR");
		}
	}

	Texture::Texture(ID id, GLuint width, GLuint height, std::vector<GLuint*> dataArray)
	{
		this->id = id;
		GLid = 0;
		array = false;

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
			this->id = CTID("ERROR");
		}

		array = true;
	}

	Texture::~Texture()
	{
		if (GLid != 0)
		{
			glDeleteTextures(1, &GLid);
			GLid = 0;
		}
	}


	/*
		Sprite
				*/
	Sprite::Sprite(ID id, std::string filepath)
	{
		this->id = id;
		GLid = 0;

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

				texCoordX = (float)width / (float)widthPT;
				texCoordY = (float)height / (float)heightPT;

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
					LOG_E << "Error loading sprite: " << gluErrorString(error);
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
			LOG_E << "Unable to load the sprite " << filepath;
			this->id = CTID("ERROR");
		}		
	}

	Sprite::~Sprite()
	{
		if (GLid != 0)
		{
			glDeleteTextures(1, &GLid);
			GLid = 0;
		}
	}


	/*
		Font
				*/
	GLuint Font::VAOid = 0;
	GLuint Font::VBOid = 0;

	Font::Font(ID id, std::string filepath, int height)
	{
		this->id = id;
		textureGLid = 0;

		//Load face
		if (FT_New_Face(assets.fontLibrary, filepath.c_str(), 0, &face))
		{
			LOG_E << "Failed to load font face from " << filepath;
			this->id = CTID("ERROR");
			return;
		}

		FT_Set_Pixel_Sizes(face, 0, height);

		//Get character information
		int maxWidth = 0;
		int maxHeight = 0;

		for (int fc = 32; fc < 256; fc++)
		{
			if (FT_Load_Char(face, fc, FT_LOAD_RENDER))
			{
				LOG_E << "Failed to load font character " << (char)fc;
			}

			FontCharacter fontChar;
			fontChar.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
			fontChar.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
			fontChar.advance = face->glyph->advance.x;

			int bufferSize = face->glyph->bitmap.width * face->glyph->bitmap.rows;
			fontChar.bitmap = new GLubyte[bufferSize];
			memcpy(fontChar.bitmap, face->glyph->bitmap.buffer, bufferSize);
			characters.insert(std::pair<GLchar, FontCharacter>((char)fc, fontChar));

			if (fontChar.size.x > maxWidth)
			{
				maxWidth = fontChar.size.x;
			}

			if (fontChar.size.y > maxHeight)
			{
				maxHeight = fontChar.size.y;
			}
		}

		maxCharWidth = maxWidth;
		maxCharHeight = maxHeight;

		maxWidth += 2;
		maxHeight += 2;

		//Store characters into texture
		textureWidth = maxWidth * 16;
		textureHeight = maxHeight * 16;

		GLubyte fontPixels[textureWidth * textureHeight];

		for (int fc = 32; fc < 256; fc++)
		{
			FontCharacter* fontChar = &characters[(GLchar)fc];

			int startX = (fc % 16) * maxWidth;
			int startY = (fc / 16) * maxHeight;

			fontChar->textureCoords = glm::vec4((float)startX / (float)textureWidth, 
												(float)(startX + fontChar->size.x) / (float)textureWidth, 
												(float)startY / (float)textureHeight, 
												(float)(startY + fontChar->size.y) / (float)textureHeight);

			for (int j = 0; j < fontChar->size.y; j++)
			{
				for (int i = 0; i < fontChar->size.x; i++)
				{
					fontPixels[((j + startY) * textureWidth) + (i + startX)] = fontChar->bitmap[(j * fontChar->size.x) + i];
				}
			}
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &textureGLid);
			glBindTexture(GL_TEXTURE_2D, textureGLid);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureWidth, textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, &fontPixels);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		FT_Done_Face(face);		
	}

	Font::~Font()
	{
		if (textureGLid != 0)
		{
			glDeleteTextures(1, &textureGLid);
			textureGLid = 0;
		}
	}

	void Font::loadFontQuadToGPU()
	{
		glGenVertexArrays(1, &VAOid);
		glGenBuffers(1, &VBOid);

		glBindVertexArray(VAOid);
			glBindBuffer(GL_ARRAY_BUFFER, VBOid);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, NULL, GL_DYNAMIC_DRAW);

				//Position
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

				//UV
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Font::clearFontQuadFromGPU()
	{
		if (VAOid != 0)
		{
			glDeleteVertexArrays(1, &VAOid);
			glDeleteBuffers(1, &VBOid);
			VAOid = 0;
			VBOid = 0;
		}
	}


	/*
		Shader
				*/
	Shader::Shader(ID id, std::string vertexShaderFilepath, std::string fragmentShaderFilepath)
	{
		this->id = id;
		GLid = 0;

		//Has shader already been built?
		if (GLid != 0)
		{
			LOG_E << "Shader already built";
			this->id = CTID("ERROR");
			return;	
		}

		//Load Shader
		GLint success;
		GLchar infoLog[512];

		//Load shader files
		const GLchar* vertexShaderSource = sc::fileRead(vertexShaderFilepath.c_str());

		if (vertexShaderSource == NULL)
		{
			LOG_E << "Failed to open vertex shader";		
			this->id = CTID("ERROR");
			return;	
		}

		const GLchar* fragmentShaderSource = sc::fileRead(fragmentShaderFilepath.c_str());

		if (fragmentShaderSource == NULL)
		{
			LOG_E << "Failed to open fragment shader";		
			this->id = CTID("ERROR");
			return;
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
			this->id = CTID("ERROR");
			return;
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
			this->id = CTID("ERROR");
			return;
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
			this->id = CTID("ERROR");
			return;		
		}

		//Clean-up Shader Code from memory
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}

	Shader::~Shader()
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
				textureMaterialArguments.push_back(assets.textureStack.get((*tma)[i]));
			}
		}

		shader = assets.shaderStack.get(shaderId);
	}

	Material::~Material() {}


	/*
		Assets
				*/
	Assets::Assets()
	{
		//Initiate FreeType
		if (FT_Init_FreeType(&fontLibrary))
		{
			LOG_E << "Error initiating FreeType";		
		}
	}

	bool Assets::areWorldAssetsLoaded()
	{
		if (meshStack.areWorldAssetsLoaded()
			|| textureStack.areWorldAssetsLoaded()
			|| spriteStack.areWorldAssetsLoaded()
			|| fontStack.areWorldAssetsLoaded()
			|| shaderStack.areWorldAssetsLoaded()
			|| materialStack.areWorldAssetsLoaded()
			|| modelStack.areWorldAssetsLoaded())
		{
			return true;
		}

		return false;
	}

	void Assets::loadBaseAssets()
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

		meshStack.pushBase(new Mesh(CTID("ME_QUAD"), &vecVert, &vecInd));
		meshStack.pushBase(new Mesh(CTID("ME_SPHERE"), "Resources/Meshes/ME_SPHERE.obj"));

		//Flat Quad
		vecVert.clear();

		tempVert.position = glm::vec3(0.0f, 0.0f, 0.0f);
		tempVert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
		tempVert.textureCoord = glm::vec2(0.0f, 0.0f);
		vecVert.push_back(tempVert);

		tempVert.position = glm::vec3(1.0f, 0.0f, 0.0f);
		tempVert.textureCoord = glm::vec2(1.0f, 0.0f);
		vecVert.push_back(tempVert);

		tempVert.position = glm::vec3(0.0f, 0.0f, 1.0f);
		tempVert.textureCoord = glm::vec2(0.0f, 1.0f);
		vecVert.push_back(tempVert);

		tempVert.position = glm::vec3(1.0f, 0.0f, 1.0f);
		tempVert.textureCoord = glm::vec2(1.0f, 1.0f);
		vecVert.push_back(tempVert);

		static const int ind2[] = {0, 2, 1, 3, 1, 2};
		std::vector<int> vecInd2(ind2, ind2 + sizeof(ind2) / sizeof(ind2[0]));

		meshStack.pushBase(new Mesh(CTID("ME_FLATQUAD"), &vecVert, &vecInd2));

		//Load Sprites
		spriteStack.pushBase(new Sprite(CTID("SP_ERROR"), "Resources/Textures/ERROR.png"));
		spriteStack.pushBase(new Sprite(CTID("SP_TEST"), "Resources/Textures/testSprite.png"));

		spriteStack.pushBase(new Sprite(CTID("SP_CLICKCUR"), "Resources/Textures/Cursor/SP_CLICKCUR.png"));
		spriteStack.pushBase(new Sprite(CTID("SP_DRAGCUR"), "Resources/Textures/Cursor/SP_DRAGCUR.png"));
		spriteStack.pushBase(new Sprite(CTID("SP_HOVERCUR"), "Resources/Textures/Cursor/SP_HOVERCUR.png"));
		spriteStack.pushBase(new Sprite(CTID("SP_POINTCUR"), "Resources/Textures/Cursor/SP_POINTCUR.png"));

		spriteStack.pushBase(new Sprite(CTID("SP_SAVEICON"), "Resources/Textures/Icons/SP_SAVEICON.png"));
		spriteStack.pushBase(new Sprite(CTID("SP_LOADICON"), "Resources/Textures/Icons/SP_LOADICON.png"));

		//Load Fonts
		fontStack.pushBase(new Font(CTID("FT_TEST"), "Resources/Fonts/OpenSans-Regular.ttf", 28));
		fontStack.pushBase(new Font(CTID("FT_MONO"), "Resources/Fonts/RobotoMono-Regular.ttf", 16));
		fontStack.pushBase(new Font(CTID("FT_EDITOR"), "Resources/Fonts/RobotoMono-Regular.ttf", 20));

		//Load textures
		textureStack.pushBase(new Texture(CTID("TX_EDITSLOTA"), "Resources/Textures/editorEmptySlot.png"));
		textureStack.pushBase(new Texture(CTID("TX_EDITSLOTB"), "Resources/Textures/editorEmptySlotSelected.png"));

		//Load Shaders
		shaderStack.pushBase(new Shader(CTID("SH_PASS"), "Resources/Shaders/sc_shader_testVertex.glsl", "Resources/Shaders/sc_shader_testFragment.glsl"));
		shaderStack.pushBase(new Shader(CTID("SH_TEX"), "Resources/Shaders/sc_shader_testTextureVertex.glsl", "Resources/Shaders/sc_shader_testTextureFragment.glsl"));
		shaderStack.pushBase(new Shader(CTID("SH_STAGE"), "Resources/Shaders/sc_shader_stageVertex.glsl", "Resources/Shaders/sc_shader_stageFragment.glsl"));
		shaderStack.pushBase(new Shader(CTID("SH_STAGESELECT"), "Resources/Shaders/sc_shader_stageSelectVertex.glsl", "Resources/Shaders/sc_shader_stageSelectFragment.glsl"));
		shaderStack.pushBase(new Shader(CTID("SH_COLOR"), "Resources/Shaders/sc_shader_flatColorVertex.glsl", "Resources/Shaders/sc_shader_flatColorFragment.glsl"));
		shaderStack.pushBase(new Shader(CTID("SH_SPRITE"), "Resources/Shaders/sc_shader_spriteVertex.glsl", "Resources/Shaders/sc_shader_spriteFragment.glsl"));
		shaderStack.pushBase(new Shader(CTID("SH_FONT"), "Resources/Shaders/sc_shader_fontVertex.glsl", "Resources/Shaders/sc_shader_fontFragment.glsl"));

		std::vector<glm::vec4> tempVec4;
		tempVec4.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		materialStack.pushBase(new Material(CTID("MA_RED"), NULL, NULL, &tempVec4, NULL, CTID("SH_PASS")));
		tempVec4.clear();
		tempVec4.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		materialStack.pushBase(new Material(CTID("MA_BLUE"), NULL, NULL, &tempVec4, NULL, CTID("SH_PASS")));

		std::vector<ID> tempTex;
		tempTex.push_back(CTID("TX_EDITSLOTA"));
		materialStack.pushBase(new Material(CTID("MA_EDITSLOTA"), NULL, NULL, NULL, &tempTex, CTID("SH_TEX")));
		tempTex.clear();
		tempTex.push_back(CTID("TX_EDITSLOTB"));
		materialStack.pushBase(new Material(CTID("MA_EDITSLOTB"), NULL, NULL, NULL, &tempTex, CTID("SH_TEX")));
		tempTex.clear();

		Font::loadFontQuadToGPU();
		FT_Done_FreeType(fontLibrary);
	}

	void Assets::clearBaseAssets()
	{
		if (areWorldAssetsLoaded())
		{
			LOG_E << "World assets haven't been properly cleared, they are going to be wiped out along with base assets";
		}

		meshStack.clearBase();
		textureStack.clearBase();
		spriteStack.clearBase();
		fontStack.clearBase();
		shaderStack.clearBase();
		materialStack.clearBase();
		modelStack.clearBase();

		Font::clearFontQuadFromGPU();
	}

	void Assets::clearWorldAssets()
	{
		meshStack.clearWorld();
		textureStack.clearWorld();
		spriteStack.clearWorld();
		fontStack.clearWorld();
		shaderStack.clearWorld();
		materialStack.clearWorld();
		modelStack.clearWorld();
	}
}