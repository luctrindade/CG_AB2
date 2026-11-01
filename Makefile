# Compilar: make        Limpar: make clean
CXX      ?= g++
CXXFLAGS  = -std=c++11 -O2 -Wall -Isrc -MMD -MP
FONTES    = main.cpp $(wildcard src/*.cpp) $(wildcard src/objetos/*.cpp)
OBJETOS   = $(FONTES:.cpp=.o)

ifeq ($(OS),Windows_NT)
  LIBS = -lfreeglut -lopengl32 -lglu32
  EXE  = cena.exe
else
  LIBS = -lglut -lGLU -lGL
  EXE  = cena
endif

$(EXE): $(OBJETOS)
	$(CXX) $(OBJETOS) -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJETOS) $(OBJETOS:.o=.d) $(EXE)

-include $(OBJETOS:.o=.d)
