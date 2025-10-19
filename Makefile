# Compiler & flags
CXX       := g++
CXXFLAGS  := -std=c++20 -Wall -Wextra -Iinclude

# Directories
SRCDIR    := src
OBJDIR    := build
TESTDIR   := tests

# Target binary name
TARGET    := kite_server

# Collect source files
SOURCES   := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS   := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# Test files
TEST_SOURCES := $(wildcard $(TESTDIR)/*.cpp)
TEST_TARGETS := $(patsubst $(TESTDIR)/%.cpp, $(OBJDIR)/%_test, $(TEST_SOURCES))

.PHONY: all tests clean

all: $(TARGET)

# Link main binary
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

# Tests target
tests: $(TEST_TARGETS)
	@echo "Running tests..."
	@for t in $(TEST_TARGETS); do \
	  echo ">>> $$t"; \
	  ./$$t; \
	done

# Build each test
$(OBJDIR)/%_test: $(TESTDIR)/%.cpp $(OBJECTS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $< $(OBJECTS) -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)
