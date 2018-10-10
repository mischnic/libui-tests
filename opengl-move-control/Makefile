CFLAGS = -I ../../libui-gl -I glew -fsanitize=address
LDFLAGS = -lui -L../../libui-gl/build/out -framework OpenGL -fsanitize=address

OS := $(shell uname)
ifeq ($(OS), Darwin)
LDFLAGS += -rpath @loader_path/../../libui-gl/build/out
endif

.PHONY: run
run: build
	./opengl

.PHONY: build
build: opengl

opengl: main.o glew/glew.c
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f opengl *.o glew/*.o

