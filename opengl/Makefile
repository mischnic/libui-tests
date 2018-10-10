CXXFLAGS = -I ../../libui-gl -I lib/glew -fsanitize=address
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

opengl: main.o lib/Mat.o lib/Vec.o lib/glew/glew.o
	$(CXX) $(LDFLAGS) $^ -o $@

clean:
	rm -f opengl *.o lib/*.o lib/glew/glew.o

main.o: lib/glew/glew.h lib/Vec.h lib/Mat.h
Vec.o: lib/glew/glew.h
Mat.o: lib/glew/glew.h lib/Vec.h
