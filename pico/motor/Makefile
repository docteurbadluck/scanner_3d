SRC_DIR   := srcs
TEST_DIR  := tests
BUILD_DIR := build_test
UNITY_DIR := tests/unity
UNITY_INC := $(UNITY_DIR)

CXX      := g++
CC       := gcc
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -I$(UNITY_INC) -I$(SRC_DIR)
CFLAGS   := -std=c11   -Wall -Wextra -Wpedantic -I$(UNITY_INC)

SRC_SOURCES := $(shell find $(SRC_DIR) -name '*.cpp' 2>/dev/null | grep -v 'main\.cpp' | grep -v '/4_drivers/')
SRC_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/srcs/%.o, $(SRC_SOURCES))

UNITY_SRC := $(UNITY_DIR)/unity.c
UNITY_OBJ := $(BUILD_DIR)/unity/unity.o

TEST_SOURCES  := $(shell find $(TEST_DIR) -name '*.cpp' 2>/dev/null | grep -v '/internal/')
TEST_BINS     := $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/tests/%, $(TEST_SOURCES))

INTERNAL_SOURCES := $(shell find $(TEST_DIR) -name '*.cpp' 2>/dev/null | grep '/internal/')
INTERNAL_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/tests/%.o, $(INTERNAL_SOURCES))

all: compile

compile: $(SRC_OBJECTS)

test: $(TEST_BINS)
	@sh makefile_scripts/test.sh $(TEST_BINS)

$(BUILD_DIR)/tests/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(TEST_DIR)/unit_test/1_domain -c $< -o $@

$(BUILD_DIR)/tests/%: $(TEST_DIR)/%.cpp $(UNITY_OBJ) $(SRC_OBJECTS) $(INTERNAL_OBJECTS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(TEST_DIR)/unit_test/1_domain $^ -o $@

$(BUILD_DIR)/srcs/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(UNITY_OBJ): $(UNITY_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/srcs $(BUILD_DIR)/unity

fclean: clean
	rm -rf $(BUILD_DIR)

re: fclean compile

.PHONY: all compile clean fclean re test