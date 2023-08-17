CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -lglfw -Iinclude

all: hello-triangle

hello-triangle: src/glad.c src/hello-triangle.c
	$(CC) $(CFLAGS) $^ -o $@