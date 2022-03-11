SOURCES=draw_pixel_art.cpp Matrix.cpp Vector.cpp shader_code.cpp ShapeGenerator.cpp
OBJECTS=draw_pixel_art.o Matrix.o Vector.o shader_code.o ShapeGenerator.o
HEADERS=Matrix.h Vector.h ShapeGenerator.h
CC=g++
CFLAGS=-lGLEW -lglut -lGLU -lGL -lm

all: draw_pixel_art

draw_pixel_art: $(OBJECTS)
	$(CC) $(SOURCES) $(CFLAGS) -o draw_pixel_art

clean:
	rm -f *.o draw_pixel_art hello_opengl Duke_Fishron
