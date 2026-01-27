# define
CXX = g++
# CXX = g++-14
USE_OPENMP = 1

CXXFLAGS = -Wall -std=c++14
INCLUDES = -I./include

# if USE_OPENMP=1，add -fopenmp
ifeq ($(USE_OPENMP),1)
    CXXFLAGS += -fopenmp
endif

MAIN_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
TARGET = $(BIN_DIR)/water.sim

# obtain source files (.cpp files)
SRCS = $(MAIN_DIR)/main.cpp \
	   $(MAIN_DIR)/sim.cpp \
	   $(MAIN_DIR)/integral.cpp \
	   $(MAIN_DIR)/boundary.cpp \
	   $(MAIN_DIR)/riemann_HLLC.cpp \
	   $(MAIN_DIR)/config.cpp \
	   $(MAIN_DIR)/setup.cpp \
	   $(MAIN_DIR)/utils.cpp 

# create object files (.o 文件)
OBJS = $(SRCS:$(MAIN_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# target
all: $(TARGET)

# compile rules
$(OBJ_DIR)/%.o: $(MAIN_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


# chain rule
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# clean 
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

# create obj directory (if not exist)
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# create bin directory (if not exist)
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

.PHONY: all clean