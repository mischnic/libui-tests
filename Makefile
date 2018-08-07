CFLAGS = -I ../libui
LDFLAGS = -lui -L../libui/build/out

OS := $(shell uname)
ifeq ($(OS), Darwin)
LDFLAGS += -rpath @loader_path/../libui/build/out
endif

clean:
	rm -f test test_area test_area_resize test_grid test_matrix test_seperator