CPP=g++
CFLAGS = -Wall -std=c++17 -g -Iinclude $$(sdl2-config --cflags)
LIBS = $$(sdl2-config --libs) -lSDL2_ttf -lSDL2_image
BUILD_DIR=build
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES=$(foreach file,$(CPP_FILES),$(BUILD_DIR)/$(file:.cpp=.o))
SOFTWARE_NAME=Demineur
RM=rm

$(shell mkdir -p $(BUILD_DIR))
$(shell mkdir -p $(BUILD_DIR)/src)

$(BUILD_DIR)/src/%.o: src/%.cpp
	$(CPP) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR)/$(SOFTWARE_NAME): $(OBJ_FILES)
	$(CPP) -o $(BUILD_DIR)/$(SOFTWARE_NAME) $(OBJ_FILES) $(LIBS)

release: $(BUILD_DIR)/$(SOFTWARE_NAME)
	$(CPP) -O3 -o $(BUILD_DIR)/$(SOFTWARE_NAME) $(OBJ_FILES) $(LIBS)

clean:
	$(RM) -r $(BUILD_DIR)