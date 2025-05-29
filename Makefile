# Email: razcohenp@gmail.com

# Makefile for Coup Card Game - C++ Implementation
# This file automates the build process for the Coup card game project

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O2
INCLUDES = -Iinclude
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

SRCDIR = src
INCDIR = include
OBJDIR = obj

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/roles/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Main target
TARGET = coup_game

# Create object directory
$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJDIR)/roles

# Compile object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Special rule for roles subdirectory
$(OBJDIR)/roles/%.o: $(SRCDIR)/roles/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Main executable with GUI
Main: $(OBJECTS) Main.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) Main.cpp $(OBJECTS) -o $(TARGET) $(LIBS)
	@echo "🎮 COUP Game compiled successfully!"
	@echo "📁 Run with: ./$(TARGET)"

# Test compilation
test: $(OBJECTS)
	@echo "🧪 Test compilation ready"

# Memory check
valgrind: Main
	@echo "🔍 Running memory check..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Demo executable for testing
demo: $(OBJECTS) Demo.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) Demo.cpp $(OBJECTS) -o demo $(LIBS)
	@echo "🧪 Demo compiled successfully!"
	@echo "📁 Run with: ./demo"

# Clean build
clean:
	rm -rf $(OBJDIR) $(TARGET)
	@echo "🧹 Build cleaned!"

# Development build with debug symbols
debug: CXXFLAGS += -DDEBUG -g3
debug: Main

.PHONY: Main test valgrind clean debug demo