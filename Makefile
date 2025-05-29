# Email: razcohenp@gmail.com

# Makefile for Coup Card Game - C++ Implementation
# This file automates the build process for the Coup card game project

# Compiler settings - using g++ with C++17 standard
CXX = g++
# Compilation flags: C++17 standard, all warnings, extra warnings, debug symbols, optimization level 2
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O2
# Include directory for header files
INCLUDES = -Iinclude
# SFML libraries for GUI functionality (graphics, window management, system utilities)
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Directory structure for organized build process
SRCDIR = src      # Source code directory containing .cpp files
INCDIR = include  # Header files directory containing .hpp files
OBJDIR = obj      # Object files directory for compiled .o files

# Automatic source file discovery - finds all .cpp files in src/ and src/roles/
SOURCES = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/roles/*.cpp)
# Generate corresponding object file paths from source files
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Final executable name for the game
TARGET = coup_game

# Create object directory structure - ensures directories exist before compilation
$(OBJDIR):
	mkdir -p $(OBJDIR)       # Create main object directory
	mkdir -p $(OBJDIR)/roles # Create roles subdirectory for role-specific object files

# Generic rule for compiling source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@  # Compile .cpp to .o with all flags

# Specialized rule for role classes in subdirectory
$(OBJDIR)/roles/%.o: $(SRCDIR)/roles/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@  # Compile role .cpp files to roles/ subdirectory

# Build main executable with GUI support - primary game application
Main: $(OBJECTS) Main.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) Main.cpp $(OBJECTS) -o $(TARGET) $(LIBS)  # Link all objects with SFML
	@echo "🎮 COUP Game compiled successfully!"
	@echo "📁 Run with: ./$(TARGET)"

# Prepare test environment - compiles all game objects for testing
test: $(OBJECTS)
	@echo "🧪 Test compilation ready"

# Memory debugging with valgrind - checks for memory leaks and errors
valgrind: Main
	@echo "🔍 Running memory check..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)  # Full memory analysis

# Build demo executable for automated testing and demonstration
demo: $(OBJECTS) Demo.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) Demo.cpp $(OBJECTS) -o demo $(LIBS)  # Create demo executable
	@echo "🧪 Demo compiled successfully!"
	@echo "📁 Run with: ./demo"

# Clean build artifacts - removes all compiled files for fresh build
clean:
	rm -rf $(OBJDIR) $(TARGET)  # Remove object directory and executable
	@echo "🧹 Build cleaned!"

# Development build with enhanced debugging - adds extra debug symbols and flags
debug: CXXFLAGS += -DDEBUG -g3  # Add DEBUG macro and maximum debug information
debug: Main                     # Build main target with debug configuration

# Phony targets - these don't create files, just execute commands
.PHONY: Main test valgrind clean debug demo