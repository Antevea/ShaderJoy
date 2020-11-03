#include <iostream>
#include <string>
#include "shaderhandler.hpp"
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>

#define W 1280
#define H 720

SDL_Window *window;
SDL_Renderer *renderer;
std::string shaderFilePath = "../src/Basic.shader";

int main(void)
{
	int running = 0;
	SDL_Event event;
	static float getTime;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	if(glcontext == NULL)
	{
		std::cerr << "Error: can't create sdl context.\n";
		return -1;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_GL_SetSwapInterval(1);
	
	GLuint glew = glewInit();
	if(glew != GLEW_OK)
	{
		std::cerr << "Error: can't init glew library.\n";
		return -1;
	}

	int glProg = glCreateProgram();
	
	ShaderHandler shaderObj(shaderFilePath, glProg);
	shaderObj.ShaderExec(GL_VERTEX_SHADER);
	shaderObj.ShaderExec(GL_FRAGMENT_SHADER);

	glLinkProgram(glProg);
	glUseProgram(glProg);

	GLint uniformTime = glGetUniformLocation(glProg, "iTime");
	if((GLuint)uniformTime == 0xFFFFFFFF || uniformTime == -1)
	{
		printf("Error in glGetUniformLocation");
		return -1;
	}

	while(running != 1)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				running = 1;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = 1;
					break;
				}
				break;
			}
		}
		getTime = SDL_GetTicks();
		glUniform1f(uniformTime, getTime * 0.0025 + 1);
		glRectf(-getTime, -getTime, getTime, getTime);
		glFlush();
		SDL_GL_SwapWindow(window);
	}
	SDL_Quit();
	return 0;
}