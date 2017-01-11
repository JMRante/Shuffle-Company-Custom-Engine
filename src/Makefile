COMPILER = g++

SDL_LIB = -LX:\Base\Dev\MinGWDevLib\i686-w64-mingw32\lib -lSDL2main -lSDL2
SDL_INCLUDE = -IX:\Base\Dev\MinGWDevLib\i686-w64-mingw32\include

FREEGLUT_LIB = -LX:\Base\Dev\MinGWDevLib\freeglut\lib -lopengl32 -lglu32
FREEGLUT_INCLUDE = -IX:\Base\Dev\MinGWDevLib\freeglut\include

GLEW_LIB = -LX:\Base\Dev\MinGWDevLib\glew-2.0.0_Source\glew-2.0.0\lib -lglew32
GLEW_INCLUDE = -IX:\Base\Dev\MinGWDevLib\glew-2.0.0_Source\glew-2.0.0\include

COMPILER_FLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE) $(FREEGLUT_INCLUDE) $(GLEW_INCLUDE)
LD_FLAGS = -lmingw32 $(SDL_LIB) $(FREEGLUT_LIB) $(GLEW_LIB)
EXE = ShuffleCompany.exe

all: $(EXE)

$(EXE): sc_main.o
		$(COMPILER) $< $(LD_FLAGS) -o $@

sc_main.o: sc_main.cpp
		$(COMPILER) $(COMPILER_FLAGS) $< -o $@

clean:
		del *.o && del $(EXE)