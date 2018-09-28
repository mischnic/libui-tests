CFLAGS = -I ../../libui
LDFLAGS = -lui -L../../libui/build/out

OS := $(shell uname)
ifeq ($(OS), Darwin)
LDFLAGS += -rpath @loader_path/../../libui/build/out
endif

test: lodepng.o test.o
	$(CC) $(LDFLAGS) $^ -o test

clean:
	rm -f test *.o