/*

	SHUFFLE COMPANY SOURCE CODE

	sc_main.cpp
	------------------------------------

	This is where it all kicks off! The "main" function resides here. It initiates the game engine
	and starts off the main game loop.

*/

#include <iostream>

#include <SDL2\SDL.h>

#include <GL\glew.h>
#include <SDL2\SDL_opengl.h>
#include <GL\glu.h>

#define MS_PER_UPDATE 15

//Structs
struct vec3
{
	float x;
	float y;
	float z;
};

//Meshes
GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

GLuint VAO;

//Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

GLuint shaderProgram;

//Function Foward Declarations
bool initiate();

void closeout();

void render();

//Global Variable Declarations
SDL_Window *window = nullptr;
SDL_GLContext glContext = nullptr;

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

	if (window == nullptr)
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

	if (glContext == nullptr)
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
	SDL_DestroyWindow(window);
	window = nullptr;

	SDL_Quit();	
}

bool loadDataToGPU()
{
	GLint success;
	GLchar infoLog[512];

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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//Load Shader
	//Pass-through, Screen Space Vertex
	GLuint vertexShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Vertex Shader Compile Error: " << infoLog << std::endl;
		return false;
	}

	//Basic Fragment
	GLuint fragmentShader;

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Fragment Shader Compile Error: " << infoLog << std::endl;
		return false;
	}

	//Link shaders into program
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Shader Program Linking Error: " << infoLog << std::endl;
		return false;		
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

void render()
{
	glUseProgram(shaderProgram);
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