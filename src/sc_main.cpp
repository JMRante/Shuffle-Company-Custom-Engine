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
#include <SDL_image.h>

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

//Shuffle Company Headers
#include "sc_shader.h"

#define MS_PER_UPDATE 15

#define PI 3.14159265

//Structs
struct vec3
{
	float x;
	float y;
	float z;
};

//Meshes
GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
};

GLuint VAO;

//Shaders
Shader testShader;

//Function Foward Declarations
bool initiate();

void closeout();

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
	window = SDL_CreateWindow("Shuffle Company", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);

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

	return true;
}

void closeout()
{
	testShader.remove();

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();	
}

bool loadDataToGPU()
{
	//Load mesh
	//Generate Vertex Buffer Object
	GLuint VBO;
	glGenBuffers(1, &VBO);

	//Generate Vertex Array Object
	glGenVertexArrays(1, &VAO);

	//Bind Vertex Array Object
	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//Position Attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		//Color Attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	return testShader.build("Shaders/sc_shader_testVertex.glsl", "Shaders/sc_shader_testFragment.glsl");
}

void update()
{
	GLfloat currentTimeMS = (float)SDL_GetTicks();
	GLfloat greenValue = (sin((currentTimeMS * 2.0 * PI) / 1000.0) / 2.0) + 0.5;
	GLint vertexColorLocation = glGetUniformLocation(testShader.programID, "ourColor");

	testShader.use();
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
}

void render()
{
	testShader.use();
	glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

int main(int argc, char **argv)
{
	if (initiate() && loadDataToGPU())
	{
		//Main Game Loop
		bool hasQuit = false;

		SDL_Event event;

		Uint32 previousTimeMS = SDL_GetTicks();
		Uint32 lagTimeMS = 0;

		while (!hasQuit)
		{
			//Get the amount of time since last render
			Uint32 currentTimeMS = SDL_GetTicks();
			Uint32 elapsedTimeMS = currentTimeMS - previousTimeMS;
			previousTimeMS = currentTimeMS;
			lagTimeMS += elapsedTimeMS;

			//Handle events
			while (SDL_PollEvent(&event) != 0)
			{
				if (event.type == SDL_QUIT)
				{
					hasQuit = true;
				}
			}

			//Update
			while (lagTimeMS >= MS_PER_UPDATE)
			{
				//update();
				lagTimeMS -= MS_PER_UPDATE;
			}

			//Render
			render();
			SDL_GL_SwapWindow(window);
		}
	}
	else
	{
		std::cout << "Initiation Error: Failed to start" << std::endl;
	}

	closeout();

	return 0;
}