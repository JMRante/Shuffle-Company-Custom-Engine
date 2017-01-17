/*

	SHUFFLE COMPANY SOURCE CODE

	sc_main.cpp
	------------------------------------

	This is where it all kicks off! The "main" function resides here. It initiates the game engine
	and starts off the main game loop.

*/

//Outside Headers
#include <iostream>
#include <cmath>

#include <SDL.h>

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include "SOIL.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Shuffle Company Headers
#include "sc_shader.h"

#define MS_PER_FRAME 16

#define PI 3.14159265

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define FOV 45.0f

//Structs
struct vec3
{
	float x;
	float y;
	float z;
};

//Meshes
GLfloat vertices[] = 
{
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = 
{
	glm::vec3( 0.0f,  0.0f,  0.0f), 
	glm::vec3( 2.0f,  5.0f, -15.0f), 
	glm::vec3(-1.5f, -2.2f, -2.5f),  
	glm::vec3(-3.8f, -2.0f, -12.3f),  
	glm::vec3( 2.4f, -0.4f, -3.5f),  
	glm::vec3(-1.7f,  3.0f, -7.5f),  
	glm::vec3( 1.3f, -2.0f, -2.5f),  
	glm::vec3( 1.5f,  2.0f, -2.5f), 
	glm::vec3( 1.5f,  0.2f, -1.5f), 
	glm::vec3(-1.3f,  1.0f, -1.5f)  
};

GLuint indices[] = 
{
    0, 1, 3, // First Triangle
    1, 2, 3  // Second Triangle
};

GLuint VAO;
GLuint VBO;
GLuint EBO;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

GLint modelLoc;
GLint viewLoc;
GLint projLoc;

//Shaders
Shader testShader;

unsigned char* texture = NULL;
Sint32 textureWidth = 0;
Sint32 textureHeight = 0;
GLuint textureID;

//Function Foward Declarations
bool initiate();

void closeout();

bool loadDataToGPU();

void update();

void render();

//Global Variable Declarations
SDL_Window *window = NULL;
SDL_GLContext glContext = NULL;

bool initiate()
{
	//Initiate SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Initiate Window
	window = SDL_CreateWindow("Shuffle Company", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

	if (window == NULL)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Initiate SDL OpenGL version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Initiate SDL OpenGL context
	glContext = SDL_GL_CreateContext(window);

	if (glContext == NULL)
	{
		std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Initiate GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();

	if (glewError != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(glewError) << std::endl;
		return false;
	}

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_DEPTH_TEST); 

	return true;
}

void closeout()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

	testShader.remove();

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();	
}

bool loadDataToGPU()
{
	//Load texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
		texture = SOIL_load_image("Resources/w5block256.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
		glGenerateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(texture);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Load mesh
	//Generate Vertex Buffer Object
	glGenBuffers(1, &VBO);

	//Generate Vertex Array Object
	glGenVertexArrays(1, &VAO);

	//Bind Vertex Array Object
	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//Position Attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		//Texture Coordinate Attributes
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	return testShader.build("Shaders/sc_shader_testVertex.glsl", "Shaders/sc_shader_testFragment.glsl");
}

void update()
{
    model = glm::rotate(model, glm::radians((GLfloat)SDL_GetTicks() * 0.01f), glm::vec3(0.5f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(FOV, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

    modelLoc = glGetUniformLocation(testShader.programID, "model");
    viewLoc = glGetUniformLocation(testShader.programID, "view");
    projLoc = glGetUniformLocation(testShader.programID, "projection"); 

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); 
}

void render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	testShader.use();
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBindVertexArray(VAO);
	for (GLuint i = 0; i < 10; i++)
	{
		glm::mat4 tempModel;
		tempModel = glm::translate(tempModel, cubePositions[i]);
		GLfloat angle = 20.0f * i;
		tempModel = glm::rotate(tempModel, angle, glm::vec3(1.0f, 0.3f, 0.5f));
    	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(tempModel));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);

	SDL_GL_SwapWindow(window);
}

int main(int argc, char **argv)
{
	if (initiate() && loadDataToGPU())
	{
		//Main Game Loop
		bool hasQuit = false;

		SDL_Event event;

		Uint32 startTime;
		Sint32 delay;

		while (!hasQuit)
		{
			startTime = SDL_GetTicks();

			//Handle events
			while (SDL_PollEvent(&event) != 0)
			{
				if (event.type == SDL_QUIT)
				{
					hasQuit = true;
				}
			}

			update();
			render();

			delay = startTime + MS_PER_FRAME - SDL_GetTicks();
			if (delay >= 0)
			{
				SDL_Delay(delay);
			}
		}
	}
	else
	{
		std::cout << "Initiation Error: Failed to start" << std::endl;
	}

	closeout();

	return 0;
}