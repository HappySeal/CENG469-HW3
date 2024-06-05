UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
CC := /usr/bin/clang++
CFLAGS := -std=c++17 -fdiagnostics-color=always -Wall -g -Idependencies/include
LDFLAGS := -Ldependencies/library
LIBS := GLEW glfw.3.4 glm
FRAMEWORKS := -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation
MAIN := main.cpp
TARGET := app
all: clean $(TARGET)

$(TARGET) : $(MAIN)
	$(CC) $(CFLAGS) ./sources/*.cpp $(LDFLAGS) $(LIBS:%=dependencies/library/lib%.dylib) $< -o $@ $(FRAMEWORKS) -Wno-deprecated

run: all
	./$(TARGET)

endif
ifeq ($(UNAME_S), Linux)
all:
	g++ main.cpp ./sources/*.cpp -o main -g -lglfw -lpthread -lX11 -ldl -lXrandr -lGLEW -lGL -DGL_SILENCE_DEPRECATION -DGLM_ENABLE_EXPERIMENTAL -I.
endif

clean:
	rm -f $(TARGET)