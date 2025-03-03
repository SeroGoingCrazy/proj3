CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra $(addprefix -I,$(INC_DIRS)) -I/opt/homebrew/opt/googletest/include -MMD -MP
CPPFLAGS = -I/opt/homebrew/opt/expat/include
LDFLAGS = -L/opt/homebrew/opt/expat/lib -L/opt/homebrew/opt/googletest/lib
LDLIBS = -lexpat -lgtest -lgtest_main -pthread

SRC_DIR = ./src
TEST_SRC_DIR = ./testsrc
OBJ_DIR = ./obj
BIN_DIR = ./bin

SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
TEST_SRCS = $(shell find $(TEST_SRC_DIR) -name '*.cpp')
OBJS = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRCS:.cpp=.o))
TEST_OBJS = $(patsubst $(TEST_SRC_DIR)/%, $(OBJ_DIR)/%, $(TEST_SRCS:.cpp=.o))
DEPS = $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)

-include $(DEPS)

INC_DIRS = ./include /opt/homebrew/opt/expat/include $(shell find $(SRC_DIR) $(TEST_SRC_DIR) -type d 2>/dev/null)

.PHONY: all clean test

all: $(BIN_DIR)/teststrdatasource $(BIN_DIR)/teststrdatasink $(BIN_DIR)/testbussystem $(BIN_DIR)/testosm $(BIN_DIR)/teststrutils

$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/teststrdatasource: $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSourceTest.o $(OBJ_DIR)/StringUtils.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(BIN_DIR)/teststrdatasink: $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSinkTest.o $(OBJ_DIR)/StringUtils.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(BIN_DIR)/testbussystem: $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/TestBusSystem.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(BIN_DIR)/testosm: $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/TestOpenStreetMap.o  $(OBJ_DIR)/StringUtils.o| $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(BIN_DIR)/teststrutils: $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringUtilsTest.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

test: all
	@$(BIN_DIR)/teststrdatasource
	@$(BIN_DIR)/teststrdatasink
	@$(BIN_DIR)/testbussystem
	@$(BIN_DIR)/testosm
	@$(BIN_DIR)/teststrutils
	

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)