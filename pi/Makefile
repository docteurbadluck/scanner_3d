SRC_DIR   := src
TEST_DIR  := tests
BUILD_DIR := build_test
COV_BUILD    := build_coverage
COV_HTML_DIR := ../../computer/www/coverage/pi
UNITY_DIR := tests/unity
UNITY_INC := $(UNITY_DIR)

CXX      := g++
CC       := gcc
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -I$(UNITY_INC) -I$(SRC_DIR)
CFLAGS   := -std=c11   -Wall -Wextra -Wpedantic -I$(UNITY_INC)
COV_FLAGS := --coverage -O0

SRC_SOURCES := $(shell find $(SRC_DIR) -name '*.cpp' 2>/dev/null | grep -v 'main\.cpp')
SRC_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/srcs/%.o, $(SRC_SOURCES))
COV_SRC_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(COV_BUILD)/srcs/%.o, $(SRC_SOURCES))

UNITY_SRC := $(UNITY_DIR)/unity.c
UNITY_OBJ := $(BUILD_DIR)/unity/unity.o
COV_UNITY_OBJ := $(COV_BUILD)/unity/unity.o

TEST_SOURCES  := $(shell find $(TEST_DIR) -name '*.cpp' 2>/dev/null | grep -v '/internal/')
TEST_BINS     := $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/tests/%, $(TEST_SOURCES))
COV_TEST_BINS := $(patsubst $(TEST_DIR)/%.cpp, $(COV_BUILD)/tests/%, $(TEST_SOURCES))

all: compile

compile: $(SRC_OBJECTS)

test: $(TEST_BINS)
	@sh makefile_scripts/test.sh $(TEST_BINS)

coverage: $(COV_TEST_BINS)
	@sh makefile_scripts/test.sh $(COV_TEST_BINS)

coverage-html: coverage
	@mkdir -p $(COV_HTML_DIR)
	python3 -m gcovr \
		--root . \
		--filter src/ \
		--object-directory $(COV_BUILD)/srcs \
		--html-details $(COV_HTML_DIR)/index.html

$(BUILD_DIR)/tests/%: $(TEST_DIR)/%.cpp $(UNITY_OBJ) $(SRC_OBJECTS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/srcs/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(UNITY_OBJ): $(UNITY_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(COV_BUILD)/tests/%: $(TEST_DIR)/%.cpp $(COV_UNITY_OBJ) $(COV_SRC_OBJECTS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(COV_FLAGS) $^ -o $@

$(COV_BUILD)/srcs/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(COV_FLAGS) -c $< -o $@

$(COV_UNITY_OBJ): $(UNITY_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/srcs $(BUILD_DIR)/unity

fclean: clean
	rm -rf $(BUILD_DIR) $(COV_BUILD)

re: fclean compile

.PHONY: all compile clean fclean re test coverage coverage-html
