# Compiler & flags
CXX       := g++
CXXFLAGS  := -std=c++20 -Wall -Wextra -Iinclude

# Directories
SRCDIR    := src
OBJDIR    := build
TESTDIR   := tests

# Target binary name
TARGET    := kite_server.exe

# Collect all source files
SOURCES   := $(wildcard $(SRCDIR)/*.cpp)

# Separate init.cpp (contains main)
INIT_SRC  := $(SRCDIR)/init.cpp
NON_INIT_SRCS := $(filter-out $(INIT_SRC), $(SOURCES))

# Object files
OBJECTS   := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(NON_INIT_SRCS))
INIT_OBJ  := $(OBJDIR)/init.o

# Test files
TEST_SOURCES := $(wildcard $(TESTDIR)/*.cpp)
TEST_TARGETS := $(patsubst $(TESTDIR)/%.cpp, $(OBJDIR)/%.o, $(TEST_SOURCES))

.PHONY: all tests clean

all: $(TARGET)

# Link main binary (includes init.o)
$(TARGET): $(OBJECTS) $(INIT_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

# === Tests ===
tests: $(TEST_TARGETS)
	@echo "Running tests..."
	@for t in $(TEST_TARGETS); do \
	  echo ">>> $$t"; \
	  ./$$t; \
	done

# Build each test binary (link all non-init source objects)
$(OBJDIR)/%.o: $(TESTDIR)/%.cpp $(OBJECTS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $< $(OBJECTS) -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)
