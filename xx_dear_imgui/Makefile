include ../config.mk

IMGUI_SRC := $(wildcard ../ext/imgui/*.cpp)
IMGUI_SRC += ../ext/intro_imgui_impl.cpp

OBJ := $(IMGUI_SRC:.cpp=.o) main.o ../ext/gl.o $(INTROLIB)
CPPFLAGS += $(INC_FLAGS) -I../ext/imgui `sdl2-config --cflags`
CFLAGS := -O2 -g
CXXFLAGS := -O2 -g

EXE := db_main

LDFLAGS := `sdl2-config --libs`

UNAME := $(shell uname -s)
ifeq (Linux,$(UNAME))
  LDFLAGS += -ldl -lGL
endif

ifeq (Windows_NT,$(OS))
  LDFLAGS += -lopengl32 -limm32 -lgdi32
endif

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $^ $(LDFLAGS) -o $@

main.o: CXXFLAGS += -Wall
main.o: main.cpp main.cpp.intro

../ext/gl.o: ../ext/gl.h
	$(CC) -xc -DGLAD_GL_IMPLEMENTATION -c $< -o $@

%.intro: %
	$(INTRO) $(CPPFLAGS) $< -o $@
