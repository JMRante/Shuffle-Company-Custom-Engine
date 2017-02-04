COMPILER = g++
VERSION = -std=c++11

SDL_LIB = -LX:\Base\Dev\MinGWDevLib\i686-w64-mingw32\lib -lSDL2main -lSDL2
SDL_INCLUDE = -IX:\Base\Dev\MinGWDevLib\i686-w64-mingw32\include\SDL2

FREEGLUT_LIB = -LX:\Base\Dev\MinGWDevLib\freeglut\lib -lopengl32 -lglu32
FREEGLUT_INCLUDE = -IX:\Base\Dev\MinGWDevLib\freeglut\include

GLEW_LIB = -LX:\Base\Dev\MinGWDevLib\glew-2.0.0_Source\glew-2.0.0\lib -lglew32
GLEW_INCLUDE = -IX:\Base\Dev\MinGWDevLib\glew-2.0.0_Source\glew-2.0.0\include

DevIL_LIB = -LX:\Base\Dev\MinGWDevLib\DevIL\lib -lDevIL -lilu
DevIL_INCLUDE = -IX:\Base\Dev\MinGWDevLib\DevIL\include

GLM_INCLUDE = -IX:\Base\Dev\MinGWDevLib\glm

OBJLOAD_INCLUDE = -IX:\Base\Dev\MinGWDevLib\tinyobjloader-master

COMPILER_FLAGS = -g -Wall -c $(SDL_INCLUDE) $(DevIL_INCLUDE) $(FREEGLUT_INCLUDE) $(GLEW_INCLUDE) $(GLM_INCLUDE) $(OBJLOAD_INCLUDE)
LD_FLAGS = -mwindows -lmingw32 $(SDL_LIB) $(DevIL_LIB) $(FREEGLUT_LIB) $(GLEW_LIB)
EXE = ShuffleCompany.exe

all: $(EXE)

$(EXE): sc_main.o sc_config.o sc_log.o sc_utility.o sc_game.o sc_renderer.o sc_assets.o sc_world.o sc_gameElement.o
		$(COMPILER) $^ $(LD_FLAGS) -o $@ $(VERSION)

sc_main.o: sc_main.cpp
		$(COMPILER) $(COMPILER_FLAGS) $< -o $@ $(VERSION)

sc_config.o: sc_config.cpp
		$(COMPILER) $(COMPILER_FLAGS) $< -o $@ $(VERSION)

sc_log.o: sc_log.cpp
		$(COMPILER) $(COMPILER_FLAGS) $< -o $@ $(VERSION)

sc_utility.o: sc_utility.cpp
		$(COMPILER) $(COMPILER_FLAGS) $< -o $@ $(VERSION)

sc_game.o: sc_game.cpp
		$(COMPILER) $(COMPILER_FLAGS) $< -o $@ $(VERSION)

sc_renderer.o: sc_renderer.cpp
		$(COMPILER) $(COMPILER_FLAGS) $< -o $@ $(VERSION)

sc_assets.o: sc_assets.cpp
		$(COMPILER) $(COMPILER_FLAGS) $< -o $@ $(VERSION)

sc_world.o: sc_world.cpp
		$(COMPILER) $(COMPILER_FLAGS) $< -o $@ $(VERSION)

sc_gameElement.o: sc_gameElement.cpp
		$(COMPILER) $(COMPILER_FLAGS) $< -o $@ $(VERSION)

clean:
		del *.o && del $(EXE)