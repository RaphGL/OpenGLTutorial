CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -lglfw -lm -Iinclude

all: hello-triangle shaders

hello-triangle: src/glad.c src/hello-triangle.c
	@mkdir -p bin
	$(CC) $(CFLAGS) $^ -o bin/$@

shaders: src/glad.c src/shaders.c
	@mkdir -p bin
	$(CC) $(CFLAGS) $^ -o bin/$@