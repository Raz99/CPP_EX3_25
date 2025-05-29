// Email: razcohenp@gmail.com

// Main.cpp - Main entry point for the Coup game application
// This file launches the graphical user interface and handles application lifecycle

#include "include/GameGUI.hpp"
#include <iostream>

using namespace coup;

int main() {
    try {
        std::cout << "🎭 Starting COUP Game with Enhanced GUI..." << std::endl;
        
        // Initialize the main game interface for user interaction
        // GameGUI handles all visual elements and user input processing
        GameGUI gui;
        
        // Attempt to load fonts and initialize the graphics subsystem
        if (!gui.initialize()) {
            std::cerr << "❌ Failed to initialize GUI!" << std::endl;
            return -1; // Exit with error code if initialization fails
        }
        
        std::cout << "✅ GUI initialized successfully!" << std::endl;
        std::cout << "🎮 Welcome to COUP!" << std::endl;
        
        // Enter the main game loop - this blocks until user exits
        gui.run();
        
    } catch (const std::exception& e) {
        // Catch any unexpected errors and display them gracefully
        std::cerr << "💥 Error: " << e.what() << std::endl;
        return -1; // Return error code to indicate abnormal termination
    }
    
    std::cout << "👋 Thanks for playing COUP!" << std::endl;
    return 0; // Successful application exit
}